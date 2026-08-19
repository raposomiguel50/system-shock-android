[CmdletBinding()]
param([string]$DepsRoot = (Join-Path (Split-Path -Parent $PSScriptRoot) '.deps'))
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
$Root = Split-Path -Parent $PSScriptRoot
$Git = (Get-Command git.exe -ErrorAction Stop).Source
New-Item -ItemType Directory -Path $DepsRoot -Force | Out-Null
function Ensure-Repo([string]$Name,[string]$Url,[string]$Ref,[string]$ExpectedCommit='') {
    $Path = Join-Path $DepsRoot $Name
    if (-not (Test-Path -LiteralPath (Join-Path $Path '.git'))) {
        & $Git clone --no-tags $Url $Path
        if ($LASTEXITCODE -ne 0) { throw "git clone failed: $Name" }
    }
    & $Git -C $Path fetch --prune origin
    if ($LASTEXITCODE -ne 0) { throw "git fetch failed: $Name" }
    & $Git -C $Path checkout --detach $Ref
    if ($LASTEXITCODE -ne 0) { throw "git checkout failed: $Name ref=$Ref" }
    $Actual = (& $Git -C $Path rev-parse HEAD).Trim()
    if ($ExpectedCommit -and $Actual -ne $ExpectedCommit) { throw "$Name commit mismatch expected=$ExpectedCommit actual=$Actual" }
    Write-Host "$Name=$Actual"
}
Ensure-Repo 'SDL2' 'https://github.com/libsdl-org/SDL.git' '5d249570393f7a37e037abf22cd6012a4cc56a71' '5d249570393f7a37e037abf22cd6012a4cc56a71'
Ensure-Repo 'SDL2_mixer' 'https://github.com/libsdl-org/SDL_mixer.git' 'release-2.8.1'
Write-Host 'BOOTSTRAP_DEPS=PASS'
