[CmdletBinding()]
param(
    [Parameter(Mandatory)]
    [ValidateSet('v1.0.0')]
    [string] $Tag,

    [Parameter(Mandatory)]
    [string] $BaselineDll,

    [Parameter(Mandatory)]
    [switch] $ConfirmCreate
)

$ErrorActionPreference = 'Stop'
. (Join-Path $PSScriptRoot 'Validation.Common.ps1')

if (-not $ConfirmCreate) {
    throw 'Baseline capture requires the explicit -ConfirmCreate switch.'
}

$repoRoot = Get-ValidationRepoRoot
$tagOutput = @(& git -C $repoRoot rev-list -n 1 $Tag 2>$null)
$gitExitCode = $LASTEXITCODE
$tagCommit = [string]($tagOutput | Select-Object -First 1)
$tagCommit = $tagCommit.Trim()
if ($gitExitCode -ne 0 -or -not $tagCommit) {
    throw "Required baseline tag '$Tag' does not exist."
}

$baselineDirectory = Join-Path $repoRoot "tests\baseline\$Tag"
if (Test-Path -LiteralPath $baselineDirectory) {
    $existing = @(Get-ChildItem -LiteralPath $baselineDirectory -Force -ErrorAction SilentlyContinue)
    if ($existing.Count -gt 0) {
        throw "Baseline directory is not empty and will not be overwritten: $baselineDirectory"
    }
}
else {
    New-Item -ItemType Directory -Path $baselineDirectory | Out-Null
}

$resolvedDll = [System.IO.Path]::GetFullPath((Join-Path $repoRoot $BaselineDll))
if (-not (Test-Path -LiteralPath $resolvedDll -PathType Leaf)) {
    throw "Baseline DLL does not exist: $resolvedDll"
}

$mainText = Get-GitFileText -RepoRoot $repoRoot -Revision $Tag -Path 'src/main.cpp'
$papyrusText = Get-GitFileText -RepoRoot $repoRoot -Revision $Tag -Path 'scripts/UncapperMCM.psc'
$apiText = Get-GitFileText -RepoRoot $repoRoot -Revision $Tag -Path 'src/UncapperAPI.cpp'
$serializationText = Get-GitFileText -RepoRoot $repoRoot -Revision $Tag -Path 'src/Serialization.cpp'
$cmakeText = Get-GitFileText -RepoRoot $repoRoot -Revision $Tag -Path 'CMakeLists.txt'

$snapshots = [ordered]@{
    'dll-exports.txt' = Get-DllExports -DllPath $resolvedDll
    'papyrus-registrations.tsv' = Get-PapyrusRegistrations -CppText $mainText
    'papyrus-declarations.txt' = Get-PapyrusDeclarations -PapyrusText $papyrusText
    'uncapper-abi-symbols.txt' = Get-UncapperAbiSymbols -CppText $apiText
    'serialization-contract.txt' = Get-SerializationContract -CppText $serializationText
    'plugin-contract.txt' = Get-PluginContract -CMakeText $cmakeText
}

foreach ($entry in $snapshots.GetEnumerator()) {
    $path = Join-Path $baselineDirectory $entry.Key
    [System.IO.File]::WriteAllText($path, (ConvertTo-NormalizedLines -Lines @($entry.Value)), [System.Text.UTF8Encoding]::new($false))
}

$sensitiveFiles = @(
    'src/Settings.cpp'
    'include/Settings.h'
    'src/Serialization.cpp'
    'include/Serialization.h'
    'src/UncapperAPI.cpp'
    'include/UncapperAPI.h'
    'scripts/UncapperMCM.psc'
    'scripts/UncapperMCMConfig.psc'
)
$hashLines = foreach ($path in $sensitiveFiles) {
    $blobOutput = @(& git -C $repoRoot rev-parse "${Tag}:$path" 2>$null)
    $gitExitCode = $LASTEXITCODE
    $blob = [string]($blobOutput | Select-Object -First 1)
    $blob = $blob.Trim()
    if ($gitExitCode -ne 0 -or -not $blob) {
        throw "Unable to resolve baseline blob for $path."
    }
    "$path`t$blob"
}
[System.IO.File]::WriteAllText(
    (Join-Path $baselineDirectory 'sensitive-files.tsv'),
    (ConvertTo-NormalizedLines -Lines $hashLines),
    [System.Text.UTF8Encoding]::new($false))

$dllHash = (Get-FileHash -LiteralPath $resolvedDll -Algorithm SHA256).Hash.ToLowerInvariant()
$metadata = @(
    "tag=$Tag"
    "tag_commit=$tagCommit"
    "baseline_dll_sha256=$dllHash"
    'note=The caller is responsible for supplying a DLL built from the baseline tag with the approved Release toolchain.'
)
[System.IO.File]::WriteAllText(
    (Join-Path $baselineDirectory 'baseline-metadata.txt'),
    (ConvertTo-NormalizedLines -Lines $metadata),
    [System.Text.UTF8Encoding]::new($false))

Write-Host "Captured immutable baseline snapshots in $baselineDirectory" -ForegroundColor Green
Write-Host 'Existing baseline files are never overwritten by this script.'
