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
$ReleaseVersion = '1.0.0'
$ReleaseTag = "v$ReleaseVersion"
$ReleaseVersionCode = 10000
$ReleasePackage = 'io.github.raposomiguel50.systemshock'
$ReleaseSignerSha256 = [Environment]::GetEnvironmentVariable('RP5NP_RELEASE_CERT_SHA256')
if ([string]::IsNullOrWhiteSpace($ReleaseSignerSha256)) {
    throw 'RP5NP_RELEASE_CERT_SHA256 must be set for the stable release build.'
}
$ReleaseSignerSha256 = ($ReleaseSignerSha256 -replace '[^0-9a-fA-F]','').ToLowerInvariant()

& (Join-Path $PSScriptRoot 'lint-powershell.ps1')
& (Join-Path $PSScriptRoot 'preflight.ps1') -Variant release -AndroidSdk $AndroidSdk -JavaHome $JavaHome -DepsRoot $DepsRoot
& (Join-Path $PSScriptRoot 'build.ps1') -Variant release -AndroidSdk $AndroidSdk -JavaHome $JavaHome -DepsRoot $DepsRoot

$BuiltApk = Join-Path $Root 'AndroidProject\app\build\outputs\apk\release\app-release.apk'
& (Join-Path $PSScriptRoot 'verify-apk.ps1') -Variant release -Apk $BuiltApk -AndroidSdk $AndroidSdk

New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
$ReleaseFileName = "SystemShock-Android-$ReleaseTag-arm64-v8a.apk"
$ReleaseApk = Join-Path $OutputDir $ReleaseFileName
Copy-Item -LiteralPath $BuiltApk -Destination $ReleaseApk -Force

$ReleaseHash = (Get-FileHash -LiteralPath $ReleaseApk -Algorithm SHA256).Hash.ToLowerInvariant()
$Commit = (& $Git -C $Root rev-parse HEAD).Trim()
if ($LASTEXITCODE -ne 0) {
    throw 'Unable to determine release source commit.'
}

$Manifest = [ordered]@{
    release = $ReleaseTag
    commit = $Commit
    package = $ReleasePackage
    versionCode = $ReleaseVersionCode
    versionName = $ReleaseVersion
    abi = 'arm64-v8a'
    applicationLabel = 'System Shock - Android'
    apk = $ReleaseFileName
    sha256 = $ReleaseHash
    signingCertificateSha256 = $ReleaseSignerSha256
    proprietaryGameDataIncluded = $false
}
$ManifestPath = Join-Path $OutputDir "SystemShock-Android-$ReleaseTag-release.json"
$HashPath = Join-Path $OutputDir "$ReleaseFileName.sha256"
$Manifest | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath $ManifestPath -Encoding utf8
Set-Content -LiteralPath $HashPath -Value "$ReleaseHash  $ReleaseFileName" -Encoding ascii

Write-Host 'RELEASE_GATE=PASS'
Write-Host "RELEASE=$ReleaseTag"
Write-Host "RELEASE_APK=$ReleaseApk"
Write-Host "RELEASE_MANIFEST=$ManifestPath"
Write-Host "RELEASE_SHA256=$ReleaseHash"
Write-Host "SIGNING_CERT_SHA256=$ReleaseSignerSha256"
Write-Host "SOURCE_COMMIT=$Commit"
