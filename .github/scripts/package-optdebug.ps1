<#
.SYNOPSIS
Packages the Win32 OptDebug executables, runtime DLLs, and matching symbols.
.DESCRIPTION
Requires nonempty build outputs for every listed file. Writes two ZIPs and
their SHA-256 checksums into OutputDirectory, replacing those files on reruns.
Other files in the input and output directories are left untouched.
.PARAMETER ReleaseTag
Existing release tag recorded in both archives' build-info.txt.
.PARAMETER Commit
Full commit SHA of the source used for the build.
.PARAMETER BinaryDirectory
Directory containing the OptDebug executable, DLL, and linker PDB outputs.
.PARAMETER OutputDirectory
Directory to receive the release assets; created if it does not exist.
#>
[CmdletBinding()]
param(
	[Parameter(Mandatory)]
	[ValidatePattern('^[^\r\n]+$')]
	[string]$ReleaseTag,
	[Parameter(Mandatory)]
	[ValidatePattern('^[0-9a-fA-F]{40}$')]
	[string]$Commit,
	[string]$BinaryDirectory = (Join-Path $PSScriptRoot `
		'../../out/build/vs2026/bin/OptDebug'),
	[string]$OutputDirectory = (Join-Path $PSScriptRoot '../../out/release')
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'
$paths = $ExecutionContext.SessionState.Path
$BinaryDirectory = $paths.GetUnresolvedProviderPathFromPSPath($BinaryDirectory)
$OutputDirectory = $paths.GetUnresolvedProviderPathFromPSPath($OutputDirectory)

$runtimeFiles = @(
	'MapServer.exe', 'CityOfHeroes.exe', 'DbServer.exe', 'pig.exe',
	'CrashRpt.dll', 'cg.dll', 'cgGL.dll',
	'NxCharacter.dll', 'PhysXCooking.dll', 'PhysXCore.dll', 'PhysXLoader.dll',
	'cudart32_30_9.dll', 'NxCooking.dll', 'physxcudart_20.dll', 'PhysXDevice.dll'
)
$symbolFiles = @(
	'MapServer.pdb', 'CityOfHeroes.pdb', 'DbServer.pdb', 'pig.pdb',
	'CrashRpt.pdb'
)

foreach ($name in $runtimeFiles + $symbolFiles) {
	$path = Join-Path $BinaryDirectory $name
	if (-not (Test-Path -LiteralPath $path -PathType Leaf)) {
		throw "Missing required OptDebug build output: $path"
	}
	if ((Get-Item -LiteralPath $path).Length -eq 0) {
		throw "Empty required OptDebug build output: $path"
	}
}

$buildInfo = @(
	"Tag: $ReleaseTag"
	"Commit: $($Commit.ToLowerInvariant())"
	'Configuration: OptDebug'
	'Architecture: Win32 (x86)'
	''
) -join "`n"

function Write-PackageArchive([string]$archivePath, [string[]]$fileNames)
{
	$stream = [IO.File]::Open($archivePath, [IO.FileMode]::Create,
		[IO.FileAccess]::Write, [IO.FileShare]::None)
	try {
		$archive = [IO.Compression.ZipArchive]::new($stream,
			[IO.Compression.ZipArchiveMode]::Create)
		try {
			foreach ($name in $fileNames) {
				$path = Join-Path $BinaryDirectory $name
				$null = [IO.Compression.ZipFileExtensions]::
					CreateEntryFromFile($archive, $path, $name,
						[IO.Compression.CompressionLevel]::Optimal)
			}
			$entry = $archive.CreateEntry('build-info.txt')
			$writer = [IO.StreamWriter]::new($entry.Open(),
				[Text.UTF8Encoding]::new($false))
			try {
				$writer.Write($buildInfo)
			} finally {
				$writer.Dispose()
			}
		} finally {
			$archive.Dispose()
		}
	} finally {
		$stream.Dispose()
	}
}

$null = [IO.Directory]::CreateDirectory($OutputDirectory)
$runtimeName = 'CoX-OptDebug-Win32.zip'
$symbolsName = 'CoX-OptDebug-Win32-symbols.zip'
Write-PackageArchive (Join-Path $OutputDirectory $runtimeName) $runtimeFiles
Write-PackageArchive (Join-Path $OutputDirectory $symbolsName) $symbolFiles

$checksums = foreach ($name in @($runtimeName, $symbolsName)) {
	$hash = Get-FileHash -LiteralPath (Join-Path $OutputDirectory $name) `
		-Algorithm SHA256
	"$($hash.Hash.ToLowerInvariant())  $name"
}
$checksumPath = Join-Path $OutputDirectory 'CoX-OptDebug-Win32-SHA256SUMS.txt'
[IO.File]::WriteAllText($checksumPath, ($checksums -join "`n") + "`n",
	[Text.UTF8Encoding]::new($false))

Get-Item -LiteralPath (Join-Path $OutputDirectory $runtimeName),
	(Join-Path $OutputDirectory $symbolsName), $checksumPath
