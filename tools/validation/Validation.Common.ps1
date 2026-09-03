Set-StrictMode -Version Latest

function Get-ValidationRepoRoot {
    $root = & git -C $PSScriptRoot rev-parse --show-toplevel 2>$null
    if ($LASTEXITCODE -ne 0 -or -not $root) {
        throw "Unable to locate the Git repository from $PSScriptRoot."
    }

    return [System.IO.Path]::GetFullPath(($root | Select-Object -First 1).Trim())
}

function Get-GitFileText {
    param(
        [Parameter(Mandatory)] [string] $RepoRoot,
        [Parameter(Mandatory)] [string] $Revision,
        [Parameter(Mandatory)] [string] $Path
    )

    $output = & git -C $RepoRoot show "${Revision}:$Path" 2>$null
    $gitExitCode = $LASTEXITCODE
    if ($gitExitCode -ne 0) {
        throw "Unable to read '$Path' from revision '$Revision'."
    }

    $text = $output | Out-String
    return $text.Replace("`r`n", "`n")
}

function Get-WorkingTreeText {
    param(
        [Parameter(Mandatory)] [string] $RepoRoot,
        [Parameter(Mandatory)] [string] $Path
    )

    $fullPath = Join-Path $RepoRoot $Path
    if (-not (Test-Path -LiteralPath $fullPath -PathType Leaf)) {
        throw "Required file does not exist: $Path"
    }

    return [System.IO.File]::ReadAllText($fullPath).Replace("`r`n", "`n")
}

function ConvertTo-NormalizedLines {
    param([Parameter(Mandatory)] [AllowEmptyCollection()] [string[]] $Lines)

    if ($Lines.Count -eq 0) {
        return ""
    }

    return (($Lines -join "`n") + "`n")
}

function Get-PapyrusRegistrations {
    param([Parameter(Mandatory)] [string] $CppText)

    $CppText = Remove-CppComments -Text $CppText
    $classConstants = @{}
    $constantPattern = [regex]'(?ms)(?<symbol>[A-Za-z_][A-Za-z0-9_:]*)\s*=\s*"(?<value>[^"]+)"\s*;'
    foreach ($match in $constantPattern.Matches($CppText)) {
        $classConstants[$match.Groups['symbol'].Value] = $match.Groups['value'].Value
    }

    $pattern = [regex]::new(
        'RegisterFunction\s*\(\s*"(?<name>[^"]+)"\s*,\s*(?<class>"[^"]+"|[A-Za-z_][A-Za-z0-9_:]*)\s*,\s*(?<callback>[A-Za-z_][A-Za-z0-9_:]*)\s*(?:,\s*(?<tasklet>true|false))?\s*\)',
        [System.Text.RegularExpressions.RegexOptions]::Singleline)

    $result = [System.Collections.Generic.List[string]]::new()
    $index = 1
    foreach ($match in $pattern.Matches($CppText)) {
        $classToken = $match.Groups['class'].Value
        if ($classToken.StartsWith('"')) {
            $className = $classToken.Trim('"')
        }
        elseif ($classConstants.ContainsKey($classToken)) {
            $className = $classConstants[$classToken]
        }
        else {
            $shortName = ($classToken -split '::')[-1]
            if ($classConstants.ContainsKey($shortName)) {
                $className = $classConstants[$shortName]
            }
            else {
                throw "Unable to resolve Papyrus class constant '$classToken'."
            }
        }

        $tasklet = if ($match.Groups['tasklet'].Success) {
            $match.Groups['tasklet'].Value
        }
        else {
            'default(false)'
        }

        $result.Add(("{0:D2}`t{1}`t{2}`t{3}" -f $index, $match.Groups['name'].Value, $className, $tasklet))
        $index++
    }

    return $result.ToArray()
}

function Get-PapyrusDeclarations {
    param([Parameter(Mandatory)] [string] $PapyrusText)

    $result = [System.Collections.Generic.List[string]]::new()
    foreach ($line in ($PapyrusText -split "`n")) {
        $withoutComment = ($line -split ';', 2)[0]
        $match = [regex]::Match(
            $withoutComment,
            '^\s*(?:(?<return>[A-Za-z][A-Za-z0-9_]*)\s+)?Function\s+(?<name>[A-Za-z_][A-Za-z0-9_]*)\s*\((?<params>[^)]*)\)\s+Global\s+Native\s*$',
            [System.Text.RegularExpressions.RegexOptions]::IgnoreCase)

        if (-not $match.Success) {
            continue
        }

        $returnType = if ($match.Groups['return'].Success) { $match.Groups['return'].Value } else { 'Void' }
        $parameters = $match.Groups['params'].Value.Trim()
        $parameters = [regex]::Replace($parameters, '\s*,\s*', ', ')
        $parameters = [regex]::Replace($parameters, '\s+', ' ')
        $result.Add("$returnType $($match.Groups['name'].Value)($parameters)")
    }

    return $result.ToArray()
}

function Get-UncapperAbiSymbols {
    param([Parameter(Mandatory)] [string] $CppText)

    $CppText = Remove-CppComments -Text $CppText
    return @(
        [regex]::Matches($CppText, '"(?<symbol>Uncapper_[A-Za-z0-9_]+)"') |
            ForEach-Object { $_.Groups['symbol'].Value } |
            Sort-Object -Unique
    )
}

function Get-SerializationContract {
    param([Parameter(Mandatory)] [string] $CppText)

    $CppText = Remove-CppComments -Text $CppText
    $id = [regex]::Match($CppText, "SERIALIZATION_ID\s*=\s*'(?<value>[A-Z0-9]{4})'").Groups['value'].Value
    $record = [regex]::Match($CppText, "RECORD_SETTINGS\s*=\s*'(?<value>[A-Z0-9]{4})'").Groups['value'].Value
    $currentAlias = [regex]::Match($CppText, 'CURRENT_RECORD_VERSION\s*=\s*RECORD_VERSION_V(?<value>[0-9]+)').Groups['value'].Value
    $versions = @(
        [regex]::Matches($CppText, 'constexpr\s+std::uint32_t\s+RECORD_VERSION_V(?<value>[0-9]+)\s*=') |
            ForEach-Object { [int]$_.Groups['value'].Value } |
            Sort-Object -Unique
    )
    $loaders = @(
        [regex]::Matches($CppText, 'bool\s+LoadVersion(?<value>[0-9]+)\s*\(') |
            ForEach-Object { [int]$_.Groups['value'].Value } |
            Sort-Object -Unique
    )
    $dispatchVersions = @(
        [regex]::Matches($CppText, 'version\s*==\s*RECORD_VERSION_V(?<value>[0-9]+)') |
            ForEach-Object { [int]$_.Groups['value'].Value } |
            Sort-Object -Unique
    )

    if (-not $id -or -not $record -or -not $currentAlias) {
        throw 'Unable to parse the serialization contract.'
    }

    return @(
        "serialization_id=$id"
        "record_type=$record"
        "current_version=$currentAlias"
        "declared_versions=$($versions -join ',')"
        "migration_loaders=$($loaders -join ',')"
        "load_dispatch_versions=$($dispatchVersions -join ',')"
    )
}

function Get-PluginContract {
    param([Parameter(Mandatory)] [string] $CMakeText)

    $project = [regex]::Match($CMakeText, '(?ms)project\s*\(\s*(?<name>[A-Za-z0-9_.-]+)').Groups['name'].Value
    $version = [regex]::Match($CMakeText, '(?ms)project\s*\([^)]*?VERSION\s+(?<version>[0-9.]+)').Groups['version'].Value
    $outputName = [regex]::Match($CMakeText, 'OUTPUT_NAME\s+"(?<name>[^"]+)"').Groups['name'].Value

    if (-not $project -or -not $version -or -not $outputName) {
        throw 'Unable to parse the CMake plugin contract.'
    }

    return @(
        "plugin_name=$project"
        "dll_name=$outputName.dll"
        "cmake_project_version=$version"
    )
}

function Find-Dumpbin {
    $command = Get-Command dumpbin.exe -ErrorAction SilentlyContinue
    if ($command) {
        return $command.Source
    }

    $vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (Test-Path -LiteralPath $vswhere) {
        $candidates = @(& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -find 'VC\Tools\MSVC\**\bin\Hostx64\x64\dumpbin.exe' 2>$null)
        $candidate = $candidates | Select-Object -First 1
        if ($candidate) {
            return $candidate.Trim()
        }
    }

    return $null
}

function Get-PEExportsNative {
    param([Parameter(Mandatory)] [string] $DllPath)

    $bytes = [System.IO.File]::ReadAllBytes($DllPath)
    if ($bytes.Length -lt 256 -or [System.BitConverter]::ToUInt16($bytes, 0) -ne 0x5A4D) {
        throw "Not a valid PE file: $DllPath"
    }

    $peOffset = [System.BitConverter]::ToInt32($bytes, 0x3C)
    if ([System.BitConverter]::ToUInt32($bytes, $peOffset) -ne 0x00004550) {
        throw "Invalid PE signature: $DllPath"
    }

    $sectionCount = [System.BitConverter]::ToUInt16($bytes, $peOffset + 6)
    $optionalSize = [System.BitConverter]::ToUInt16($bytes, $peOffset + 20)
    $optionalOffset = $peOffset + 24
    $magic = [System.BitConverter]::ToUInt16($bytes, $optionalOffset)
    $dataDirectoryOffset = if ($magic -eq 0x20B) { $optionalOffset + 112 } elseif ($magic -eq 0x10B) { $optionalOffset + 96 } else { throw 'Unsupported PE optional-header format.' }
    $exportRva = [System.BitConverter]::ToUInt32($bytes, $dataDirectoryOffset)
    if ($exportRva -eq 0) {
        return @()
    }

    $sections = @()
    $sectionOffset = $optionalOffset + $optionalSize
    for ($i = 0; $i -lt $sectionCount; $i++) {
        $offset = $sectionOffset + (40 * $i)
        $sections += [pscustomobject]@{
            VirtualSize = [System.BitConverter]::ToUInt32($bytes, $offset + 8)
            VirtualAddress = [System.BitConverter]::ToUInt32($bytes, $offset + 12)
            RawSize = [System.BitConverter]::ToUInt32($bytes, $offset + 16)
            RawOffset = [System.BitConverter]::ToUInt32($bytes, $offset + 20)
        }
    }

    $rvaToOffset = {
        param([uint32] $Rva)
        foreach ($section in $sections) {
            $size = [Math]::Max([uint32]$section.VirtualSize, [uint32]$section.RawSize)
            if ($Rva -ge $section.VirtualAddress -and $Rva -lt ($section.VirtualAddress + $size)) {
                return [int]($section.RawOffset + ($Rva - $section.VirtualAddress))
            }
        }
        throw "Unable to map PE RVA 0x$($Rva.ToString('X8'))."
    }

    $exportOffset = & $rvaToOffset $exportRva
    $nameCount = [System.BitConverter]::ToUInt32($bytes, $exportOffset + 24)
    $namesRva = [System.BitConverter]::ToUInt32($bytes, $exportOffset + 32)
    $namesOffset = & $rvaToOffset $namesRva
    $exports = [System.Collections.Generic.List[string]]::new()

    for ($i = 0; $i -lt $nameCount; $i++) {
        $nameRva = [System.BitConverter]::ToUInt32($bytes, $namesOffset + (4 * $i))
        $nameOffset = & $rvaToOffset $nameRva
        $end = $nameOffset
        while ($end -lt $bytes.Length -and $bytes[$end] -ne 0) { $end++ }
        $exports.Add([System.Text.Encoding]::ASCII.GetString($bytes, $nameOffset, $end - $nameOffset))
    }

    return @($exports | Sort-Object -Unique)
}

function Get-DllExports {
    param(
        [Parameter(Mandatory)] [string] $DllPath,
        [ValidateSet('Auto', 'Dumpbin', 'NativePE')] [string] $Tool = 'Auto'
    )

    if (-not (Test-Path -LiteralPath $DllPath -PathType Leaf)) {
        throw "DLL does not exist: $DllPath"
    }

    $dumpbin = Find-Dumpbin
    if ($Tool -eq 'Dumpbin' -and -not $dumpbin) {
        throw 'dumpbin.exe was requested but could not be found.'
    }

    if ($Tool -ne 'NativePE' -and $dumpbin) {
        $output = & $dumpbin /nologo /exports $DllPath 2>&1
        if ($LASTEXITCODE -ne 0) {
            if ($Tool -eq 'Dumpbin') {
                throw "dumpbin failed for $DllPath."
            }
        }
        else {
            return @(
                $output |
                    ForEach-Object {
                        if ($_ -match '^\s+\d+\s+[0-9A-Fa-f]+\s+[0-9A-Fa-f]+\s+(?<name>[^\s=]+)') { $Matches['name'] }
                    } |
                    Where-Object { $_ } |
                    Sort-Object -Unique
            )
        }
    }

    return @(Get-PEExportsNative -DllPath $DllPath)
}

function Remove-CppComments {
    param([Parameter(Mandatory)] [string] $Text)

    $withoutBlocks = [regex]::Replace($Text, '/\*.*?\*/', '', [System.Text.RegularExpressions.RegexOptions]::Singleline)
    return [regex]::Replace($withoutBlocks, '//.*$', '', [System.Text.RegularExpressions.RegexOptions]::Multiline)
}

function Get-SettingsStorageContract {
    param([Parameter(Mandatory)] [string] $CppText)

    $code = Remove-CppComments -Text $CppText
    $definitions = [regex]::Matches(
        $code,
        '(?m)^\s*(?!extern\b)(?:static\s+)?(?:Settings::)?Data\s+g_settings\s*(?:[;={])')
    $externs = [regex]::Matches(
        $code,
        '(?m)^\s*extern\b[^;\r\n]*\bg_settings\b')

    return [pscustomobject]@{
        DefinitionCount = $definitions.Count
        ExternCount = $externs.Count
    }
}

function Compare-NormalizedSnapshot {
    param(
        [Parameter(Mandatory)] [string] $ExpectedPath,
        [Parameter(Mandatory)] [string[]] $ActualLines
    )

    if (-not (Test-Path -LiteralPath $ExpectedPath -PathType Leaf)) {
        return $false
    }

    $expected = [System.IO.File]::ReadAllText($ExpectedPath).Replace("`r`n", "`n").TrimEnd("`n")
    $actual = ($ActualLines -join "`n").TrimEnd("`n")
    return $expected -ceq $actual
}
