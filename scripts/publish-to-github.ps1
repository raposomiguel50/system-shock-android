[CmdletBinding()]
param(
    [Parameter(Mandatory=$true)][string]$RepositoryUrl,
    [string]$CommitMessage = 'Initial public System Shock Android source snapshot'
)
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
$Root = Split-Path -Parent $PSScriptRoot
$Git = (Get-Command git.exe -ErrorAction Stop).Source
Push-Location $Root
try {
    if (Test-Path -LiteralPath '.git') { throw 'This source folder already contains .git. Publish from a fresh extraction or use Git manually.' }
    & $Git init -b main
    if ($LASTEXITCODE -ne 0) { throw 'git init failed' }
    & $Git add --all
    if ($LASTEXITCODE -ne 0) { throw 'git add failed' }
    & $Git commit -m $CommitMessage
    if ($LASTEXITCODE -ne 0) { throw 'git commit failed. Check your Git user.name/user.email configuration.' }
    & $Git remote add origin $RepositoryUrl
    if ($LASTEXITCODE -ne 0) { throw 'git remote add failed' }
    & $Git push -u origin main
    if ($LASTEXITCODE -ne 0) { throw 'git push failed. Check that the GitHub repository exists, is empty, and that Git authentication is configured.' }
    Write-Host 'PUBLIC_REPOSITORY_PUSH=PASS'
    Write-Host "REPOSITORY=$RepositoryUrl"
} finally { Pop-Location }
