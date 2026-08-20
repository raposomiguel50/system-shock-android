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

& (Join-Path $PSScriptRoot 'lint-powershell.ps1')
& (Join-Path $PSScriptRoot 'preflight.ps1') -Variant release -AndroidSdk $AndroidSdk -JavaHome $JavaHome -DepsRoot $DepsRoot
& (Join-Path $PSScriptRoot 'build.ps1') -Variant release -AndroidSdk $AndroidSdk -JavaHome $JavaHome -DepsRoot $DepsRoot

$BuiltApk = Join-Path $Root 'AndroidProject\app\build\outputs\apk\release\app-release.apk'
& (Join-Path $PSScriptRoot 'verify-apk.ps1') -Variant release -Apk $BuiltApk -AndroidSdk $AndroidSdk

New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
$ReleaseFileName = 'SystemShock-Android-v0.1.0-pre.3-arm64-v8a.apk'
$ReleaseApk = Join-Path $OutputDir $ReleaseFileName
Copy-Item -LiteralPath $BuiltApk -Destination $ReleaseApk -Force

$ReleaseHash = (Get-FileHash -LiteralPath $ReleaseApk -Algorithm SHA256).Hash.ToLowerInvariant()
$Commit = (& $Git -C $Root rev-parse HEAD).Trim()
if ($LASTEXITCODE -ne 0) {
    throw 'Unable to determine release source commit.'
}

$Manifest = [ordered]@{
    release = 'v0.1.0-pre.3'
    commit = $Commit
    package = 'com.rp5np.systemshock'
    versionCode = 13
    versionName = '0.1.0-pre.3'
    abi = 'arm64-v8a'
    apk = $ReleaseFileName
    sha256 = $ReleaseHash
}
$ManifestPath = Join-Path $OutputDir 'SystemShock-Android-v0.1.0-pre.3-release.json'
$HashPath = Join-Path $OutputDir "$ReleaseFileName.sha256"
$Manifest | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath $ManifestPath -Encoding utf8
Set-Content -LiteralPath $HashPath -Value "$ReleaseHash  $ReleaseFileName" -Encoding ascii

Write-Host 'RELEASE_GATE=PASS'
Write-Host "RELEASE_APK=$ReleaseApk"
Write-Host "RELEASE_MANIFEST=$ManifestPath"
Write-Host "RELEASE_SHA256=$ReleaseHash"
Write-Host "SOURCE_COMMIT=$Commit"
