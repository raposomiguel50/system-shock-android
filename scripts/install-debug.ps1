[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)][string]$GameRes,
    [string]$Apk = ''
)
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
$Root = Split-Path -Parent $PSScriptRoot
if (-not $Apk) { $Apk = Join-Path $Root 'AndroidProject\app\build\outputs\apk\debug\app-debug.apk' }
if (-not (Test-Path -LiteralPath $Apk -PathType Leaf)) { throw "APK not found: $Apk" }
$Data = Join-Path $GameRes 'data'
$Sound = Join-Path $GameRes 'sound'
foreach ($P in @($Data,$Sound)) { if (-not (Test-Path -LiteralPath $P -PathType Container)) { throw "Required game-data directory missing: $P" } }
$Adb = (Get-Command adb.exe -ErrorAction Stop).Source
$State = (& $Adb get-state 2>&1 | Out-String).Trim()
if ($State -ne 'device') { throw "ADB device not ready: $State" }
& $Adb install -r $Apk
if ($LASTEXITCODE -ne 0) { throw "adb install failed: $LASTEXITCODE" }
$Pkg='io.github.raposomiguel50.systemshock'
$Tmp='/data/local/tmp/systemshock-android-res'
& $Adb shell am force-stop $Pkg | Out-Null
& $Adb shell rm -rf $Tmp | Out-Null
& $Adb shell mkdir -p $Tmp | Out-Null
& $Adb push $Data "$Tmp/data"
if ($LASTEXITCODE -ne 0) { throw 'adb push data failed' }
& $Adb push $Sound "$Tmp/sound"
if ($LASTEXITCODE -ne 0) { throw 'adb push sound failed' }
& $Adb shell chmod -R a+rX $Tmp | Out-Null
& $Adb shell run-as $Pkg rm -rf files/res
if ($LASTEXITCODE -ne 0) { throw 'run-as unavailable. This helper requires a debuggable build.' }
& $Adb shell run-as $Pkg mkdir -p files/res
if ($LASTEXITCODE -ne 0) { throw 'Failed to create app-private res directory' }
& $Adb shell run-as $Pkg cp -R "$Tmp/data" files/res/data
if ($LASTEXITCODE -ne 0) { throw 'Failed to copy res/data into app-private storage' }
& $Adb shell run-as $Pkg cp -R "$Tmp/sound" files/res/sound
if ($LASTEXITCODE -ne 0) { throw 'Failed to copy res/sound into app-private storage' }
& $Adb shell rm -rf $Tmp | Out-Null
& $Adb shell am start -n "$Pkg/com.rp5np.systemshock.ShockolateActivity"
if ($LASTEXITCODE -ne 0) { throw 'Application launch failed' }
Write-Host 'INSTALL_DEBUG_AND_DATA=PASS'
