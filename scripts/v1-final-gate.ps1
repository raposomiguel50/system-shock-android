[CmdletBinding()]
param(
    [string]$AndroidSdk = '',
    [string]$JavaHome = $env:JAVA_HOME,
    [string]$DepsRoot = (Join-Path (Split-Path -Parent $PSScriptRoot) '.deps'),
    [string]$OutputDir = (Join-Path (Split-Path -Parent $PSScriptRoot) 'dist')
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

$Root = Split-Path -Parent $PSScriptRoot
$Git = (Get-Command git -ErrorAction Stop).Source
$ExpectedVersion = '1.0.0'
$ExpectedVersionCode = 10000
$ExpectedPackage = 'com.rp5np.systemshock'
$ExpectedAbi = 'arm64-v8a'
$ExpectedLabel = 'System Shock - Android'
$ExpectedSigner = '7419c3aae7efaeea3e0e10945a98164418faf92fa1e55deac2b654c72cb34409'

$Head = (& $Git -C $Root rev-parse HEAD).Trim()
if ($LASTEXITCODE -ne 0 -or [string]::IsNullOrWhiteSpace($Head)) {
    throw 'Unable to determine source commit.'
}
$Branch = (& $Git -C $Root branch --show-current).Trim()
if ($LASTEXITCODE -ne 0) {
    throw 'Unable to determine current branch.'
}

& $Git -C $Root diff --quiet --exit-code
if ($LASTEXITCODE -ne 0) {
    throw 'Tracked working-tree changes detected. Run the v1 gate from a clean checkout.'
}
& $Git -C $Root diff --cached --quiet --exit-code
if ($LASTEXITCODE -ne 0) {
    throw 'Staged source changes detected. Run the v1 gate from a clean checkout.'
}

Write-Host "V1_SOURCE_COMMIT=$Head"
Write-Host "V1_SOURCE_BRANCH=$Branch"

& (Join-Path $PSScriptRoot 'qa-gate.ps1') -AndroidSdk $AndroidSdk -JavaHome $JavaHome -DepsRoot $DepsRoot
& (Join-Path $PSScriptRoot 'release-gate.ps1') -AndroidSdk $AndroidSdk -JavaHome $JavaHome -DepsRoot $DepsRoot -OutputDir $OutputDir

$ReleaseTag = "v$ExpectedVersion"
$ApkName = "SystemShock-Android-$ReleaseTag-arm64-v8a.apk"
$ApkPath = Join-Path $OutputDir $ApkName
$HashPath = Join-Path $OutputDir "$ApkName.sha256"
$ManifestPath = Join-Path $OutputDir "SystemShock-Android-$ReleaseTag-release.json"

foreach ($Path in @($ApkPath, $HashPath, $ManifestPath)) {
    if (-not (Test-Path -LiteralPath $Path -PathType Leaf)) {
        throw "Expected v1 release artifact missing: $Path"
    }
}

$Manifest = Get-Content -LiteralPath $ManifestPath -Raw | ConvertFrom-Json
if ($Manifest.release -ne $ReleaseTag) { throw "Release manifest tag mismatch: $($Manifest.release)" }
if ($Manifest.commit -ne $Head) { throw "Release manifest commit mismatch. Expected $Head, found $($Manifest.commit)" }
if ($Manifest.package -ne $ExpectedPackage) { throw "Release manifest package mismatch: $($Manifest.package)" }
if ([int]$Manifest.versionCode -ne $ExpectedVersionCode) { throw "Release manifest versionCode mismatch: $($Manifest.versionCode)" }
if ($Manifest.versionName -ne $ExpectedVersion) { throw "Release manifest versionName mismatch: $($Manifest.versionName)" }
if ($Manifest.abi -ne $ExpectedAbi) { throw "Release manifest ABI mismatch: $($Manifest.abi)" }
if ($Manifest.applicationLabel -ne $ExpectedLabel) { throw "Release manifest label mismatch: $($Manifest.applicationLabel)" }
if ($Manifest.signingCertificateSha256 -ne $ExpectedSigner) { throw "Release manifest signing certificate mismatch: $($Manifest.signingCertificateSha256)" }
if ($Manifest.proprietaryGameDataIncluded -ne $false) { throw 'Release manifest must state proprietaryGameDataIncluded=false.' }

$ActualApkHash = (Get-FileHash -LiteralPath $ApkPath -Algorithm SHA256).Hash.ToLowerInvariant()
if ($Manifest.sha256 -ne $ActualApkHash) {
    throw "APK hash does not match release manifest. Manifest=$($Manifest.sha256) actual=$ActualApkHash"
}
$PublishedHashLine = (Get-Content -LiteralPath $HashPath -Raw).Trim()
$ExpectedHashLine = "$ActualApkHash  $ApkName"
if ($PublishedHashLine -ne $ExpectedHashLine) {
    throw "APK .sha256 file mismatch. Expected '$ExpectedHashLine', found '$PublishedHashLine'"
}

$ReportPath = Join-Path $OutputDir "SystemShock-Android-$ReleaseTag-final-gate.txt"
$Report = @(
    'V1_FINAL_LOCAL_GATE=PASS'
    "GENERATED_UTC=$((Get-Date).ToUniversalTime().ToString('o'))"
    "SOURCE_COMMIT=$Head"
    "SOURCE_BRANCH=$Branch"
    "RELEASE=$ReleaseTag"
    "VERSION_CODE=$ExpectedVersionCode"
    "PACKAGE=$ExpectedPackage"
    "ABI=$ExpectedAbi"
    "APPLICATION_LABEL=$ExpectedLabel"
    "APK=$ApkName"
    "APK_SHA256=$ActualApkHash"
    "SIGNING_CERT_SHA256=$ExpectedSigner"
    'PROPRIETARY_GAME_DATA_INCLUDED=false'
    'MANUAL_REFERENCE_PLAYTHROUGH=accepted_before_source_freeze'
)
$Report | Set-Content -LiteralPath $ReportPath -Encoding utf8

Write-Host 'V1_FINAL_LOCAL_GATE=PASS'
Write-Host "V1_REPORT=$ReportPath"
Write-Host "V1_APK=$ApkPath"
Write-Host "V1_APK_SHA256=$ActualApkHash"
Write-Host "V1_SOURCE_COMMIT=$Head"
