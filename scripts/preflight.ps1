[CmdletBinding()]
param(
    [ValidateSet('debug','qa','release')][string]$Variant = 'debug',
    [string]$AndroidSdk = '',
    [string]$JavaHome = $env:JAVA_HOME,
    [string]$DepsRoot = (Join-Path (Split-Path -Parent $PSScriptRoot) '.deps'),
    [switch]$SkipGitCleanCheck
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$Root = Split-Path -Parent $PSScriptRoot
$Git = (Get-Command git -ErrorAction Stop).Source

if (-not $AndroidSdk) {
    if ($env:ANDROID_SDK_ROOT) { $AndroidSdk = $env:ANDROID_SDK_ROOT }
    elseif ($env:ANDROID_HOME) { $AndroidSdk = $env:ANDROID_HOME }
    elseif ($env:LOCALAPPDATA) { $AndroidSdk = Join-Path $env:LOCALAPPDATA 'Android\Sdk' }
}

if (-not $JavaHome) {
    throw 'JDK 17 required. Set JAVA_HOME or pass -JavaHome.'
}

$Java = Join-Path $JavaHome 'bin\java.exe'
if (-not (Test-Path -LiteralPath $Java -PathType Leaf)) {
    throw "java.exe not found: $Java"
}

$JavaVersionLine = (& $Java -version 2>&1 | Select-Object -First 1).ToString()
if ($JavaVersionLine -notmatch 'version\s+"(?<major>\d+)') {
    throw "Unable to determine Java major version from: $JavaVersionLine"
}
$JavaMajor = [int]$Matches.major
if ($JavaMajor -ne 17) {
    throw "JDK 17 required. Detected Java $JavaMajor at '$JavaHome'."
}

if (-not (Test-Path -LiteralPath $AndroidSdk -PathType Container)) {
    throw "Android SDK not found: $AndroidSdk"
}

$RequiredAndroidPaths = @(
    (Join-Path $AndroidSdk 'platforms\android-34'),
    (Join-Path $AndroidSdk 'build-tools\34.0.0'),
    (Join-Path $AndroidSdk 'ndk\29.0.14206865'),
    (Join-Path $AndroidSdk 'cmake\3.22.1')
)
foreach ($RequiredPath in $RequiredAndroidPaths) {
    if (-not (Test-Path -LiteralPath $RequiredPath -PathType Container)) {
        throw "Required Android component missing: $RequiredPath"
    }
}

$GradleWrapper = Join-Path $Root 'AndroidProject\gradlew.bat'
if (-not (Test-Path -LiteralPath $GradleWrapper -PathType Leaf)) {
    throw "Gradle wrapper missing: $GradleWrapper"
}

$DependencyExpectations = @(
    [pscustomobject]@{
        Name = 'SDL2'
        Path = (Join-Path $DepsRoot 'SDL2')
        Commit = '5d249570393f7a37e037abf22cd6012a4cc56a71'
    },
    [pscustomobject]@{
        Name = 'SDL2_mixer'
        Path = (Join-Path $DepsRoot 'SDL2_mixer')
        Commit = '171eb2d420d5643e4ee11514a06e04a41a463bbd'
    }
)

foreach ($Dependency in $DependencyExpectations) {
    $CMakeLists = Join-Path $Dependency.Path 'CMakeLists.txt'
    if (-not (Test-Path -LiteralPath $CMakeLists -PathType Leaf)) {
        throw "Dependency missing: $($Dependency.Name). Run scripts/bootstrap-deps.ps1."
    }
    $ActualCommit = (& $Git -C $Dependency.Path rev-parse HEAD).Trim()
    if ($LASTEXITCODE -ne 0) {
        throw "Unable to read dependency commit: $($Dependency.Name)"
    }
    if ($ActualCommit -ne $Dependency.Commit) {
        throw "$($Dependency.Name) commit mismatch. Expected $($Dependency.Commit), found $ActualCommit"
    }
}

if (-not $SkipGitCleanCheck) {
    & $Git -C $Root diff --quiet --exit-code
    if ($LASTEXITCODE -ne 0) {
        throw 'Tracked working-tree changes detected. Build from a clean source checkout.'
    }
    & $Git -C $Root diff --cached --quiet --exit-code
    if ($LASTEXITCODE -ne 0) {
        throw 'Staged source changes detected. Build from a clean source checkout.'
    }
}

if ($Variant -eq 'release') {
    $RequiredSigningVariables = @(
        'RP5NP_RELEASE_STORE_FILE',
        'RP5NP_RELEASE_STORE_PASSWORD',
        'RP5NP_RELEASE_KEY_ALIAS',
        'RP5NP_RELEASE_KEY_PASSWORD'
    )
    foreach ($VariableName in $RequiredSigningVariables) {
        $Value = [Environment]::GetEnvironmentVariable($VariableName)
        if ([string]::IsNullOrWhiteSpace($Value)) {
            throw "Release signing variable missing: $VariableName"
        }
    }
    $ReleaseStoreFile = [Environment]::GetEnvironmentVariable('RP5NP_RELEASE_STORE_FILE')
    if (-not (Test-Path -LiteralPath $ReleaseStoreFile -PathType Leaf)) {
        throw "Release keystore not found: $ReleaseStoreFile"
    }
}

Write-Host 'PREFLIGHT=PASS'
Write-Host "VARIANT=$Variant"
Write-Host "JAVA_MAJOR=$JavaMajor"
Write-Host "JAVA_HOME=$JavaHome"
Write-Host "ANDROID_SDK=$AndroidSdk"
Write-Host "SDL2_COMMIT=$($DependencyExpectations[0].Commit)"
Write-Host "SDL2_MIXER_COMMIT=$($DependencyExpectations[1].Commit)"
if ($Variant -eq 'release') {
    Write-Host 'RELEASE_SIGNING=CONFIGURED'
}
