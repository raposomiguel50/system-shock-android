[CmdletBinding()]
param(
    [ValidateSet('debug','qa','release')][string]$Variant = 'debug',
    [string]$AndroidSdk = '',
    [string]$JavaHome = $env:JAVA_HOME,
    [string]$DepsRoot = (Join-Path (Split-Path -Parent $PSScriptRoot) '.deps')
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$Root = Split-Path -Parent $PSScriptRoot
$Preflight = Join-Path $PSScriptRoot 'preflight.ps1'

if (-not $AndroidSdk) {
    if ($env:ANDROID_SDK_ROOT) { $AndroidSdk = $env:ANDROID_SDK_ROOT }
    elseif ($env:ANDROID_HOME) { $AndroidSdk = $env:ANDROID_HOME }
    elseif ($env:LOCALAPPDATA) { $AndroidSdk = Join-Path $env:LOCALAPPDATA 'Android\Sdk' }
}

& $Preflight -Variant $Variant -AndroidSdk $AndroidSdk -JavaHome $JavaHome -DepsRoot $DepsRoot

$Sdl = Join-Path $DepsRoot 'SDL2'
$Mixer = Join-Path $DepsRoot 'SDL2_mixer'

$env:JAVA_HOME = $JavaHome
$env:ANDROID_HOME = $AndroidSdk
$env:ANDROID_SDK_ROOT = $AndroidSdk
$env:RP5NP_SDL2_SOURCE_DIR = $Sdl
$env:RP5NP_SDL2_MIXER_SOURCE_DIR = $Mixer

$VariantInfo = switch ($Variant) {
    'debug' {
        [pscustomobject]@{
            Task = ':app:assembleDebug'
            Apk = 'app\build\outputs\apk\debug\app-debug.apk'
        }
    }
    'qa' {
        [pscustomobject]@{
            Task = ':app:assembleQa'
            Apk = 'app\build\outputs\apk\qa\app-qa.apk'
        }
    }
    'release' {
        [pscustomobject]@{
            Task = ':app:assembleRelease'
            Apk = 'app\build\outputs\apk\release\app-release.apk'
        }
    }
}

$AndroidProject = Join-Path $Root 'AndroidProject'
Push-Location $AndroidProject
try {
    & .\gradlew.bat --no-daemon $VariantInfo.Task
    if ($LASTEXITCODE -ne 0) {
        throw "Gradle build failed: $LASTEXITCODE"
    }
}
finally {
    Pop-Location
}

$Apk = Join-Path $AndroidProject $VariantInfo.Apk
if (-not (Test-Path -LiteralPath $Apk -PathType Leaf)) {
    throw "Expected APK not found: $Apk"
}

Write-Host 'BUILD=PASS'
Write-Host "VARIANT=$Variant"
Write-Host "APK=$Apk"
Write-Host "SHA256=$((Get-FileHash -LiteralPath $Apk -Algorithm SHA256).Hash.ToLowerInvariant())"
