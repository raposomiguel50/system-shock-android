[CmdletBinding()]
param(
    [string]$Apk = '',
    [string]$AndroidSdk = '',
    [int]$SmokeSeconds = 10,
    [string]$LogcatOut = ''
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$Root = Split-Path -Parent $PSScriptRoot
$QaPackage = 'io.github.raposomiguel50.systemshock.qa'
$LegacyBaselinePackage = 'com.rp5np.systemshock'

if ($SmokeSeconds -lt 1 -or $SmokeSeconds -gt 120) {
    throw 'SmokeSeconds must be between 1 and 120.'
}

if (-not $AndroidSdk) {
    if ($env:ANDROID_SDK_ROOT) { $AndroidSdk = $env:ANDROID_SDK_ROOT }
    elseif ($env:ANDROID_HOME) { $AndroidSdk = $env:ANDROID_HOME }
    elseif ($env:LOCALAPPDATA) { $AndroidSdk = Join-Path $env:LOCALAPPDATA 'Android\Sdk' }
}
if (-not (Test-Path -LiteralPath $AndroidSdk -PathType Container)) {
    throw "Android SDK not found: $AndroidSdk"
}

$Adb = Join-Path $AndroidSdk 'platform-tools\adb.exe'
if (-not (Test-Path -LiteralPath $Adb -PathType Leaf)) {
    throw "adb.exe not found: $Adb"
}

if (-not $Apk) {
    $Apk = Join-Path $Root 'AndroidProject\app\build\outputs\apk\qa\app-qa.apk'
}
if (-not (Test-Path -LiteralPath $Apk -PathType Leaf)) {
    throw "QA APK not found: $Apk"
}

if ($LogcatOut) {
    $LogcatDirectory = Split-Path -Parent $LogcatOut
    if ($LogcatDirectory) { New-Item -ItemType Directory -Force -Path $LogcatDirectory | Out-Null }
}

function Get-PackageSnapshot {
    param([Parameter(Mandatory=$true)][string]$PackageName)
    $PathLines = @(& $Adb shell pm path $PackageName 2>$null | ForEach-Object { $_.ToString().Trim() } | Where-Object { $_ })
    $DumpLines = @(& $Adb shell dumpsys package $PackageName 2>$null | ForEach-Object { $_.ToString() })
    $VersionCodeLine = @($DumpLines | Select-String -Pattern 'versionCode=' | Select-Object -First 1)
    $VersionNameLine = @($DumpLines | Select-String -Pattern 'versionName=' | Select-Object -First 1)
    return [pscustomobject]@{
        Path = ($PathLines -join "`n")
        VersionCode = if ($VersionCodeLine.Count -gt 0) { $VersionCodeLine[0].ToString().Trim() } else { '' }
        VersionName = if ($VersionNameLine.Count -gt 0) { $VersionNameLine[0].ToString().Trim() } else { '' }
    }
}

$DeviceLines = @(& $Adb devices -l | ForEach-Object { $_.ToString() })
$ReadyDevices = @($DeviceLines | Where-Object { $_ -match '\sdevice\s' })
if ($ReadyDevices.Count -ne 1) { throw "Expected exactly one authorized ADB device. Found $($ReadyDevices.Count)." }

$BaselineBefore = Get-PackageSnapshot -PackageName $LegacyBaselinePackage
if ([string]::IsNullOrWhiteSpace($BaselineBefore.Path)) {
    throw "Historical baseline package is not installed: $LegacyBaselinePackage"
}

& (Join-Path $PSScriptRoot 'verify-apk.ps1') -Variant qa -Apk $Apk -AndroidSdk $AndroidSdk
& (Join-Path $PSScriptRoot 'install-qa-from-baseline.ps1') -Apk $Apk -AndroidSdk $AndroidSdk

$BaselineAfterInstall = Get-PackageSnapshot -PackageName $LegacyBaselinePackage
if ($BaselineBefore.Path -ne $BaselineAfterInstall.Path -or
    $BaselineBefore.VersionCode -ne $BaselineAfterInstall.VersionCode -or
    $BaselineBefore.VersionName -ne $BaselineAfterInstall.VersionName) {
    throw 'Historical baseline package metadata changed after QA installation.'
}

& $Adb shell am force-stop $QaPackage | Out-Null
& $Adb logcat -c
if ($LASTEXITCODE -ne 0) { throw 'Unable to clear logcat before QA smoke test.' }

& $Adb shell am start -n "$QaPackage/com.rp5np.systemshock.ShockolateActivity"
if ($LASTEXITCODE -ne 0) { throw 'Unable to launch QA package for smoke test.' }

Start-Sleep -Seconds $SmokeSeconds
$QaPid = (& $Adb shell pidof $QaPackage 2>$null | Out-String).Trim()
if ($QaPid -notmatch '^\d+(\s+\d+)*$') { throw "QA process was not alive after $SmokeSeconds seconds. pidof output: $QaPid" }

$LogcatLines = @(& $Adb logcat -d -t 1500 | ForEach-Object { $_.ToString() })
if ($LogcatOut) { $LogcatLines | Set-Content -LiteralPath $LogcatOut -Encoding utf8 }
$FatalLines = @($LogcatLines | Where-Object {
    $_ -match 'io\.github\.raposomiguel50\.systemshock\.qa' -and
    $_ -match '(FATAL EXCEPTION|Fatal signal|SIGSEGV|SIGABRT)'
})
if ($FatalLines.Count -gt 0) { throw "Fatal QA logcat entries detected:`n$($FatalLines -join "`n")" }

$BaselineAfterSmoke = Get-PackageSnapshot -PackageName $LegacyBaselinePackage
if ($BaselineBefore.Path -ne $BaselineAfterSmoke.Path -or
    $BaselineBefore.VersionCode -ne $BaselineAfterSmoke.VersionCode -or
    $BaselineBefore.VersionName -ne $BaselineAfterSmoke.VersionName) {
    throw 'Historical baseline package metadata changed during QA smoke test.'
}

Write-Host 'DEVICE_QA_GATE=PASS'
Write-Host "ADB_DEVICE=$($ReadyDevices[0].Trim())"
Write-Host "QA_PACKAGE=$QaPackage"
Write-Host "LEGACY_BASELINE_PACKAGE=$LegacyBaselinePackage"
Write-Host "QA_PID=$QaPid"
Write-Host "SMOKE_SECONDS=$SmokeSeconds"
if ($LogcatOut) { Write-Host "LOGCAT=$LogcatOut" }
Write-Host 'LEGACY_BASELINE_PRESERVED=PASS'
