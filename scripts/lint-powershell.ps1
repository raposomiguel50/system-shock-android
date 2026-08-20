[CmdletBinding()]
param(
    [string]$ScriptsPath = (Join-Path (Split-Path -Parent $PSScriptRoot) 'scripts'),
    [switch]$RequirePSScriptAnalyzer
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

if (-not (Test-Path -LiteralPath $ScriptsPath -PathType Container)) {
    throw "Scripts directory not found: $ScriptsPath"
}

$ScriptFiles = @(Get-ChildItem -LiteralPath $ScriptsPath -Filter '*.ps1' -File | Sort-Object FullName)
if ($ScriptFiles.Count -lt 1) {
    throw "No PowerShell scripts found in: $ScriptsPath"
}

$ParseFailures = [System.Collections.Generic.List[string]]::new()
foreach ($ScriptFile in $ScriptFiles) {
    $Tokens = $null
    $ParseErrors = $null
    [void][System.Management.Automation.Language.Parser]::ParseFile(
        $ScriptFile.FullName,
        [ref]$Tokens,
        [ref]$ParseErrors
    )
    foreach ($ParseError in @($ParseErrors)) {
        $ParseFailures.Add("$($ScriptFile.Name):$($ParseError.Extent.StartLineNumber): $($ParseError.Message)")
    }
}
if ($ParseFailures.Count -gt 0) {
    throw "PowerShell parse failures:`n$($ParseFailures -join "`n")"
}

$Analyzer = Get-Command Invoke-ScriptAnalyzer -ErrorAction SilentlyContinue
if (-not $Analyzer) {
    if ($RequirePSScriptAnalyzer) {
        throw 'PSScriptAnalyzer is required but Invoke-ScriptAnalyzer is not available.'
    }
    Write-Host 'PSSCRIPTANALYZER=NOT_INSTALLED'
}
else {
    $Rules = @(
        'PSAvoidAssignmentToAutomaticVariable',
        'PSAvoidUsingInvokeExpression'
    )
    $AnalyzerFindings = @(
        Invoke-ScriptAnalyzer -Path $ScriptsPath -Recurse -IncludeRule $Rules
    )
    if ($AnalyzerFindings.Count -gt 0) {
        $Messages = @($AnalyzerFindings | ForEach-Object {
            "$($_.ScriptName):$($_.Line):$($_.Column) [$($_.RuleName)] $($_.Message)"
        })
        throw "PSScriptAnalyzer failures:`n$($Messages -join "`n")"
    }
    Write-Host 'PSSCRIPTANALYZER=PASS'
}

Write-Host 'POWERSHELL_PARSE=PASS'
Write-Host "SCRIPTS_CHECKED=$($ScriptFiles.Count)"
Write-Host 'POWERSHELL_LINT=PASS'
