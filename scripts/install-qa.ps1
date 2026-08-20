[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)][string]$GameRes,
    [string]$Apk = '',
    [string]$AndroidSdk = ''
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$Root = Split-Path -Parent $PSScriptRoot
$QaPackage = 'com.rp5np.systemshock.qa'
$BaselinePackage = 'com.rp5np.systemshock'

if (-not $AndroidSdk) {
    if ($env:ANDROID_SDK_ROOT) { $AndroidSdk = $env:ANDROID_SDK_ROOT }
    elseif ($env:ANDROID_HOME) { $AndroidSdk = $env:ANDROID_HOME }
    elseif ($env:LOCALAPPDATA) { $AndroidSdk = Join-Path $env:LOCALAPPDATA 'Android\Sdk' }
}

if (-not $Apk) {
    $Apk = Join-Path $Root 'AndroidProject\app\build\outputs\apk\qa\app-qa.apk'
}
if (-not (Test-Path -LiteralPath $Apk -PathType Leaf)) {
    throw "QA APK not found: $Apk"
}

$Data = Join-Path $GameRes 'data'
$Sound = Join-Path $GameRes 'sound'
foreach ($GameDataPath in @($Data,$Sound)) {
    if (-not (Test-Path -LiteralPath $GameDataPath -PathType Container)) {
        throw "Required game-data directory missing: $GameDataPath"
    }
}

& (Join-Path $PSScriptRoot 'verify-apk.ps1') -Variant qa -Apk $Apk -AndroidSdk $AndroidSdk

$Adb = Join-Path $AndroidSdk 'platform-tools\adb.exe'
if (-not (Test-Path -LiteralPath $Adb -PathType Leaf)) {
    throw "adb.exe not found: $Adb"
}

$DeviceState = (& $Adb get-state 2>&1 | Out-String).Trim()
if ($DeviceState -ne 'device') {
    throw "ADB device not ready: $DeviceState"
}

$BaselinePathBefore = @(& $Adb shell pm path $BaselinePackage 2>$null | ForEach-Object { $_.ToString().Trim() })

& $Adb install -r $Apk
if ($LASTEXITCODE -ne 0) {
    throw "QA APK install failed: $LASTEXITCODE"
}

$TempDataRoot = '/data/local/tmp/rp5np-systemshock-qa-res'
& $Adb shell am force-stop $QaPackage | Out-Null
& $Adb shell rm -rf $TempDataRoot | Out-Null
& $Adb shell mkdir -p $TempDataRoot | Out-Null

& $Adb push $Data "$TempDataRoot/data"
if ($LASTEXITCODE -ne 0) {
    throw 'adb push data failed.'
}
& $Adb push $Sound "$TempDataRoot/sound"
if ($LASTEXITCODE -ne 0) {
    throw 'adb push sound failed.'
}
& $Adb shell chmod -R a+rX $TempDataRoot | Out-Null

& $Adb shell run-as $QaPackage rm -rf files/res
if ($LASTEXITCODE -ne 0) {
    throw 'run-as unavailable for QA package.'
}
& $Adb shell run-as $QaPackage mkdir -p files/res
if ($LASTEXITCODE -ne 0) {
    throw 'Failed to create QA app-private res directory.'
}
& $Adb shell run-as $QaPackage cp -R "$TempDataRoot/data" files/res/data
if ($LASTEXITCODE -ne 0) {
    throw 'Failed to copy res/data into QA app-private storage.'
}
& $Adb shell run-as $QaPackage cp -R "$TempDataRoot/sound" files/res/sound
if ($LASTEXITCODE -ne 0) {
    throw 'Failed to copy res/sound into QA app-private storage.'
}
& $Adb shell rm -rf $TempDataRoot | Out-Null

$BaselinePathAfter = @(& $Adb shell pm path $BaselinePackage 2>$null | ForEach-Object { $_.ToString().Trim() })
if (($BaselinePathBefore -join "`n") -ne ($BaselinePathAfter -join "`n")) {
    throw 'Installed baseline package changed during QA installation. Stop testing and inspect the device.'
}

$QaPath = @(& $Adb shell pm path $QaPackage 2>$null | ForEach-Object { $_.ToString().Trim() })
if ($QaPath.Count -lt 1) {
    throw 'QA package was not found after installation.'
}

& $Adb shell am start -n "$QaPackage/com.rp5np.systemshock.ShockolateActivity"
if ($LASTEXITCODE -ne 0) {
    throw 'QA application launch failed.'
}

Write-Host 'INSTALL_QA_AND_DATA=PASS'
Write-Host "QA_PACKAGE=$QaPackage"
Write-Host "BASELINE_PACKAGE=$BaselinePackage"
Write-Host 'BASELINE_PRESERVED=PASS'
