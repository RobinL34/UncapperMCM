[CmdletBinding()]
param(
    [string] $BaselineDirectory,
    [string] $ReleaseDll = 'build\Release\UncapperMCM.dll',
    [switch] $SkipSensitiveFiles,
    [ValidateSet('Auto', 'Dumpbin', 'NativePE')] [string] $ExportTool = 'Auto'
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'Validation.Common.ps1')

$repoRoot = Get-ValidationRepoRoot
if (-not $BaselineDirectory) {
    $BaselineDirectory = Join-Path $repoRoot 'tests\baseline\v1.0.0'
}
elseif (-not [System.IO.Path]::IsPathRooted($BaselineDirectory)) {
    $BaselineDirectory = Join-Path $repoRoot $BaselineDirectory
}
$BaselineDirectory = [System.IO.Path]::GetFullPath($BaselineDirectory)

$failures = 0
function Write-CheckResult {
    param([string] $Name, [bool] $Passed, [string] $Detail = '')
    if ($Passed) {
        Write-Host "PASS  $Name" -ForegroundColor Green
    }
    else {
        $script:failures++
        Write-Host "FAIL  $Name" -ForegroundColor Red
    }
    if ($Detail) { Write-Host "      $Detail" }
}

function Invoke-Check {
    param([string] $Name, [scriptblock] $Action)
    try {
        $result = & $Action
        Write-CheckResult -Name $Name -Passed ([bool]$result)
    }
    catch {
        Write-CheckResult -Name $Name -Passed $false -Detail $_.Exception.Message
    }
}

Write-Host "UncapperMCM refactor validation against $BaselineDirectory"

$dllPath = [System.IO.Path]::GetFullPath((Join-Path $repoRoot $ReleaseDll))
Invoke-Check 'Release DLL exists' { Test-Path -LiteralPath $dllPath -PathType Leaf }
Invoke-Check 'DLL exports match baseline' {
    $exports = Get-DllExports -DllPath $dllPath -Tool $ExportTool
    Compare-NormalizedSnapshot -ExpectedPath (Join-Path $BaselineDirectory 'dll-exports.txt') -ActualLines $exports
}

$cppFiles = @(Get-ChildItem -LiteralPath (Join-Path $repoRoot 'src') -Recurse -File -Filter '*.cpp' | Sort-Object FullName)
$cppText = ($cppFiles | ForEach-Object { [System.IO.File]::ReadAllText($_.FullName) }) -join "`n"
$registrations = @(Get-PapyrusRegistrations -CppText $cppText)
Invoke-Check 'Papyrus registrations match baseline (name, class, order, tasklet)' {
    Compare-NormalizedSnapshot -ExpectedPath (Join-Path $BaselineDirectory 'papyrus-registrations.tsv') -ActualLines $registrations
}
Invoke-Check 'Papyrus registration count is 50' { $registrations.Count -eq 50 }
Invoke-Check 'Papyrus class is UncapperMCM' {
    @($registrations | Where-Object { ($_ -split "`t")[2] -cne 'UncapperMCM' }).Count -eq 0
}
Invoke-Check 'Papyrus registrations remain non-tasklet by default' {
    @($registrations | Where-Object { ($_ -split "`t")[3] -cne 'default(false)' }).Count -eq 0
}

$papyrusText = Get-WorkingTreeText -RepoRoot $repoRoot -Path 'scripts/UncapperMCM.psc'
$declarations = @(Get-PapyrusDeclarations -PapyrusText $papyrusText)
Invoke-Check 'Papyrus declarations and signatures match baseline' {
    Compare-NormalizedSnapshot -ExpectedPath (Join-Path $BaselineDirectory 'papyrus-declarations.txt') -ActualLines $declarations
}
Invoke-Check 'Papyrus native declaration count is 50' { $declarations.Count -eq 50 }

$registeredNames = @($registrations | ForEach-Object { ($_ -split "`t")[1] })
$declaredNames = @($declarations | ForEach-Object { if ($_ -match '^\S+\s+(?<name>[^ (]+)\(') { $Matches['name'] } })
Invoke-Check 'Every declared native is registered, with no extras' {
    @($declaredNames | Where-Object { $_ -notin $registeredNames }).Count -eq 0 -and
    @($registeredNames | Where-Object { $_ -notin $declaredNames }).Count -eq 0
}

$apiText = Get-WorkingTreeText -RepoRoot $repoRoot -Path 'src/UncapperAPI.cpp'
$abiSymbols = @(Get-UncapperAbiSymbols -CppText $apiText)
Invoke-Check 'SkyrimUncapper ABI symbol names match baseline' {
    Compare-NormalizedSnapshot -ExpectedPath (Join-Path $BaselineDirectory 'uncapper-abi-symbols.txt') -ActualLines $abiSymbols
}
Invoke-Check 'SkyrimUncapper ABI symbol count is 61' { $abiSymbols.Count -eq 61 }

$serializationText = Get-WorkingTreeText -RepoRoot $repoRoot -Path 'src/Serialization.cpp'
$serializationContract = @(Get-SerializationContract -CppText $serializationText)
Invoke-Check 'Serialization contract and V1-V7 loaders match baseline' {
    Compare-NormalizedSnapshot -ExpectedPath (Join-Path $BaselineDirectory 'serialization-contract.txt') -ActualLines $serializationContract
}

$cmakeText = Get-WorkingTreeText -RepoRoot $repoRoot -Path 'CMakeLists.txt'
$pluginContract = @(Get-PluginContract -CMakeText $cmakeText)
Invoke-Check 'Plugin and DLL naming contract matches baseline' {
    Compare-NormalizedSnapshot -ExpectedPath (Join-Path $BaselineDirectory 'plugin-contract.txt') -ActualLines $pluginContract
}

$storage = Get-SettingsStorageContract -CppText $cppText
Invoke-Check 'Exactly one g_settings definition exists' { $storage.DefinitionCount -eq 1 }
Invoke-Check 'No extern g_settings declaration exists' { $storage.ExternCount -eq 0 }

if ($SkipSensitiveFiles) {
    Write-Host 'SKIP  Sensitive files unchanged from v1.0.0 (explicitly disabled)' -ForegroundColor Yellow
}
else {
    Invoke-Check 'Sensitive files unchanged from v1.0.0' {
        $snapshot = Join-Path $BaselineDirectory 'sensitive-files.tsv'
        if (-not (Test-Path -LiteralPath $snapshot -PathType Leaf)) { return $false }
        foreach ($line in [System.IO.File]::ReadAllLines($snapshot)) {
            if (-not $line.Trim()) { continue }
            $parts = $line -split "`t", 2
            if ($parts.Count -ne 2) { return $false }
            $hashOutput = @(& git -C $repoRoot hash-object -- $parts[0] 2>$null)
            $gitExitCode = $LASTEXITCODE
            $actual = [string]($hashOutput | Select-Object -First 1)
            $actual = $actual.Trim()
            if ($gitExitCode -ne 0 -or $actual -cne $parts[1]) { return $false }
        }
        return $true
    }
}

Invoke-Check 'git diff --check' {
    & git -C $repoRoot diff --check
    return $LASTEXITCODE -eq 0
}

if ($failures -gt 0) {
    Write-Host "Validation failed: $failures check(s) failed." -ForegroundColor Red
    exit 1
}

Write-Host 'Validation passed: all enabled checks passed.' -ForegroundColor Green
exit 0
