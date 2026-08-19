[CmdletBinding()]
param(
    [string]$AndroidSdk = '',
    [string]$JavaHome = $env:JAVA_HOME,
    [string]$DepsRoot = (Join-Path (Split-Path -Parent $PSScriptRoot) '.deps')
)
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
$Root = Split-Path -Parent $PSScriptRoot
if (-not $AndroidSdk) {
    if ($env:ANDROID_SDK_ROOT) { $AndroidSdk = $env:ANDROID_SDK_ROOT }
    elseif ($env:ANDROID_HOME) { $AndroidSdk = $env:ANDROID_HOME }
    elseif ($env:LOCALAPPDATA) { $AndroidSdk = Join-Path $env:LOCALAPPDATA 'Android\Sdk' }
}
if (-not $JavaHome) { throw 'JDK 17 required. Set JAVA_HOME or pass -JavaHome.' }
$Java = Join-Path $JavaHome 'bin\java.exe'
if (-not (Test-Path -LiteralPath $Java -PathType Leaf)) { throw "java.exe not found: $Java" }
$JavaVersion = (& $Java -version 2>&1 | Select-Object -First 1).ToString()
if ($JavaVersion -notmatch 'version\s+"(?<major>\d+)') { throw "Unable to determine Java major version from: $JavaVersion" }
$JavaMajor = [int]$Matches.major
if ($JavaMajor -ne 17) { throw "JDK 17 required for this Gradle 8.1.1 build. Detected Java $JavaMajor at '$JavaHome'. Pass -JavaHome with a JDK 17 installation." }
Write-Host "JAVA_MAJOR=$JavaMajor"
Write-Host "JAVA_HOME=$JavaHome"
if (-not (Test-Path -LiteralPath $AndroidSdk -PathType Container)) { throw "Android SDK not found: $AndroidSdk" }
$Required = @(
    (Join-Path $AndroidSdk 'platforms\android-34'),
    (Join-Path $AndroidSdk 'build-tools\34.0.0'),
    (Join-Path $AndroidSdk 'ndk\29.0.14206865'),
    (Join-Path $AndroidSdk 'cmake\3.22.1')
)
foreach ($P in $Required) { if (-not (Test-Path -LiteralPath $P)) { throw "Required Android component missing: $P" } }
$Sdl = Join-Path $DepsRoot 'SDL2'
$Mixer = Join-Path $DepsRoot 'SDL2_mixer'
foreach ($P in @($Sdl,$Mixer)) { if (-not (Test-Path -LiteralPath (Join-Path $P 'CMakeLists.txt'))) { throw "Dependency missing; run scripts/bootstrap-deps.ps1 first: $P" } }
$env:JAVA_HOME = $JavaHome
$env:ANDROID_HOME = $AndroidSdk
$env:ANDROID_SDK_ROOT = $AndroidSdk
$env:RP5NP_SDL2_SOURCE_DIR = $Sdl
$env:RP5NP_SDL2_MIXER_SOURCE_DIR = $Mixer
$AndroidProject = Join-Path $Root 'AndroidProject'
Push-Location $AndroidProject
try {
    & .\gradlew.bat --no-daemon :app:assembleDebug
    if ($LASTEXITCODE -ne 0) { throw "Gradle build failed: $LASTEXITCODE" }
} finally { Pop-Location }
$Apk = Join-Path $AndroidProject 'app\build\outputs\apk\debug\app-debug.apk'
if (-not (Test-Path -LiteralPath $Apk -PathType Leaf)) { throw "Expected APK not found: $Apk" }
Write-Host 'BUILD=PASS'
Write-Host "APK=$Apk"
Write-Host "SHA256=$((Get-FileHash -LiteralPath $Apk -Algorithm SHA256).Hash.ToLowerInvariant())"
