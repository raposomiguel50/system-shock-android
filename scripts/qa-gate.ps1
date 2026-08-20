[CmdletBinding()]
param(
    [string]$AndroidSdk = '',
    [string]$JavaHome = $env:JAVA_HOME,
    [string]$DepsRoot = (Join-Path (Split-Path -Parent $PSScriptRoot) '.deps')
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

& (Join-Path $PSScriptRoot 'lint-powershell.ps1')
& (Join-Path $PSScriptRoot 'bootstrap-deps.ps1') -DepsRoot $DepsRoot
& (Join-Path $PSScriptRoot 'build.ps1') -Variant qa -AndroidSdk $AndroidSdk -JavaHome $JavaHome -DepsRoot $DepsRoot
& (Join-Path $PSScriptRoot 'verify-apk.ps1') -Variant qa -AndroidSdk $AndroidSdk

Write-Host 'QA_GATE=PASS'
