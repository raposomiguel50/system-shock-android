[CmdletBinding()]
param([string]$Apk = '')
Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
$Root = Split-Path -Parent $PSScriptRoot
if (-not $Apk) { $Apk = Join-Path $Root 'AndroidProject\app\build\outputs\apk\debug\app-debug.apk' }
if (-not (Test-Path -LiteralPath $Apk -PathType Leaf)) { throw "APK not found: $Apk" }
Add-Type -AssemblyName System.IO.Compression.FileSystem
$Zip = [IO.Compression.ZipFile]::OpenRead($Apk)
try {
    $Names = @($Zip.Entries | ForEach-Object FullName)
    foreach ($Required in @('lib/arm64-v8a/libmain.so','lib/arm64-v8a/libSDL2.so','lib/arm64-v8a/libSDL2_mixer.so')) {
        if ($Names -notcontains $Required) { throw "Required ARM64 library missing: $Required" }
    }
    $Bad = @($Names | Where-Object { $_ -match '(?i)(^|/)(res/data|res/sound)/|\.res$|archive\.dat$' })
    if ($Bad.Count) { throw "Potential proprietary game data found in APK: $($Bad -join ', ')" }
    $OtherAbi = @($Names | Where-Object { $_ -match '^lib/(?!arm64-v8a/)[^/]+/' })
    if ($OtherAbi.Count) { Write-Warning "Unexpected non-ARM64 libraries found: $($OtherAbi -join ', ')" }
} finally { $Zip.Dispose() }
Write-Host 'APK_VERIFY=PASS'
Write-Host "SHA256=$((Get-FileHash -LiteralPath $Apk -Algorithm SHA256).Hash.ToLowerInvariant())"
