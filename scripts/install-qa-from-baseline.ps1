[CmdletBinding()]
param(
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
if (-not (Test-Path -LiteralPath $AndroidSdk -PathType Container)) {
    throw "Android SDK not found: $AndroidSdk"
}

if (-not $Apk) {
    $Apk = Join-Path $Root 'AndroidProject\app\build\outputs\apk\qa\app-qa.apk'
}
if (-not (Test-Path -LiteralPath $Apk -PathType Leaf)) {
    throw "QA APK not found: $Apk"
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

function Get-PackagePath {
    param([Parameter(Mandatory=$true)][string]$PackageName)
    return @(& $Adb shell pm path $PackageName 2>$null | ForEach-Object { $_.ToString().Trim() } | Where-Object { $_ })
}

function Assert-RunAs {
    param([Parameter(Mandatory=$true)][string]$PackageName)
    $Output = (& $Adb shell run-as $PackageName id 2>&1 | Out-String).Trim()
    if ($LASTEXITCODE -ne 0 -or $Output -notmatch 'uid=') {
        throw "run-as unavailable for package $PackageName. Output: $Output"
    }
}

function Test-PrivateDirectory {
    param(
        [Parameter(Mandatory=$true)][string]$PackageName,
        [Parameter(Mandatory=$true)][string]$Directory
    )
    & $Adb shell run-as $PackageName test -d $Directory | Out-Null
    return ($LASTEXITCODE -eq 0)
}

function Get-PrivateFileCount {
    param(
        [Parameter(Mandatory=$true)][string]$PackageName,
        [Parameter(Mandatory=$true)][string]$Directory
    )
    $RemoteCommand = "run-as $PackageName find $Directory -type f"
    $Lines = @(& $Adb shell $RemoteCommand 2>&1 | ForEach-Object { $_.ToString().Trim() } | Where-Object { $_ })
    if ($LASTEXITCODE -ne 0) {
        throw "Unable to enumerate files for $PackageName/$Directory. Output: $($Lines -join '; ')"
    }
    return $Lines.Count
}

$BaselinePathBefore = @(Get-PackagePath -PackageName $BaselinePackage)
if ($BaselinePathBefore.Count -lt 1) {
    throw "Baseline package is not installed: $BaselinePackage"
}

Assert-RunAs -PackageName $BaselinePackage

if (-not (Test-PrivateDirectory -PackageName $BaselinePackage -Directory 'files/res/data')) {
    throw 'Baseline app-private files/res/data is unavailable through run-as.'
}
if (-not (Test-PrivateDirectory -PackageName $BaselinePackage -Directory 'files/res/sound')) {
    throw 'Baseline app-private files/res/sound is unavailable through run-as.'
}

$TarHelp = (& $Adb shell toybox tar --help 2>&1 | Out-String)
if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrWhiteSpace($TarHelp)) {
    throw 'Android toybox tar is unavailable; refusing an unverified data-copy path.'
}

$BaselineDataCount = Get-PrivateFileCount -PackageName $BaselinePackage -Directory 'files/res/data'
$BaselineSoundCount = Get-PrivateFileCount -PackageName $BaselinePackage -Directory 'files/res/sound'
if ($BaselineDataCount -lt 1 -or $BaselineSoundCount -lt 1) {
    throw 'Baseline game-data directories are unexpectedly empty.'
}

& $Adb install -r $Apk
if ($LASTEXITCODE -ne 0) {
    throw "QA APK install failed: $LASTEXITCODE"
}

Assert-RunAs -PackageName $QaPackage

& $Adb shell am force-stop $QaPackage | Out-Null
& $Adb shell run-as $QaPackage rm -rf files/res
if ($LASTEXITCODE -ne 0) {
    throw 'Unable to clear QA app-private res directory.'
}
& $Adb shell run-as $QaPackage mkdir -p files
if ($LASTEXITCODE -ne 0) {
    throw 'Unable to prepare QA app-private files directory.'
}

$CopyCommand = "run-as $BaselinePackage toybox tar -C files -cf - res | run-as $QaPackage toybox tar -C files -xf -"
$CopyOutput = @(& $Adb shell $CopyCommand 2>&1 | ForEach-Object { $_.ToString() })
if ($LASTEXITCODE -ne 0) {
    throw "On-device baseline-to-QA game-data copy failed. Output: $($CopyOutput -join '; ')"
}

$QaDataCount = Get-PrivateFileCount -PackageName $QaPackage -Directory 'files/res/data'
$QaSoundCount = Get-PrivateFileCount -PackageName $QaPackage -Directory 'files/res/sound'
if ($QaDataCount -ne $BaselineDataCount -or $QaSoundCount -ne $BaselineSoundCount) {
    throw "QA game-data count mismatch. data $QaDataCount/$BaselineDataCount, sound $QaSoundCount/$BaselineSoundCount"
}

$BaselinePathAfter = @(Get-PackagePath -PackageName $BaselinePackage)
if (($BaselinePathBefore -join "`n") -ne ($BaselinePathAfter -join "`n")) {
    throw 'Baseline package path changed during QA deployment.'
}

& $Adb shell am start -n "$QaPackage/com.rp5np.systemshock.ShockolateActivity"
if ($LASTEXITCODE -ne 0) {
    throw 'QA application launch failed.'
}

Write-Host 'INSTALL_QA_FROM_BASELINE=PASS'
Write-Host "QA_PACKAGE=$QaPackage"
Write-Host "BASELINE_PACKAGE=$BaselinePackage"
Write-Host "DATA_FILES=$QaDataCount"
Write-Host "SOUND_FILES=$QaSoundCount"
Write-Host 'BASELINE_PRESERVED=PASS'
