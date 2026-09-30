param(
	[Parameter(Mandatory)][string]$Baseline,
	[Parameter(Mandatory)][string]$Refactor,
	[Parameter(Mandatory)][string]$Data,
	[Parameter(Mandatory)][string]$Capture,
	[Parameter(Mandatory)][string]$Output,
	[string]$Endpoint = 'Connecting to dbserver',
	[string]$PhaseStart = '',
	[string[]]$Arguments = @('-nogui', '1'),
	[switch]$CaptureStdout,
	[switch]$AcknowledgeDriverWarning,
	[int]$Runs = 6,
	[int]$TimeoutSeconds = 120
)
# Run hidden children in the data directory, alternating the variants.
# Run zero warms caches; compare the medians of the remaining five pairs.
# For clients use CaptureStdout and AcknowledgeDriverWarning. For shader
# timings set PhaseStart='Render features:' and
# Endpoint='Renderer initialization complete'; use shaderCache 0/1 for
# separate cold/warm runs. Never include a failed endpoint in a comparison.
$ErrorActionPreference = 'Stop'
$outputPath = [IO.Path]::GetFullPath($Output)
New-Item -ItemType Directory -Path $outputPath -Force | Out-Null
$observations = @()
$executables = @{ baseline = $Baseline; refactor = $Refactor }
foreach ($run in 0..($Runs - 1)) {
	$variants = if ($run % 2) { @('refactor', 'baseline') } else {
		@('baseline', 'refactor')
	}
	foreach ($variant in $variants) {
		$executable = [IO.Path]::GetFullPath($executables[$variant])
		$log = Join-Path $outputPath "$variant-$run.log"
		if (Test-Path -LiteralPath $log) { Remove-Item -LiteralPath $log }
		$timer = [Diagnostics.Stopwatch]::StartNew()
		$launch = @{
			FilePath = $executable; WorkingDirectory = $Data
			ArgumentList = $Arguments; WindowStyle = 'Hidden'; PassThru = $true
		}
		if ($CaptureStdout) {
			$launch.RedirectStandardOutput = $log
			$launch.RedirectStandardError = "$log.stderr"
		}
		$child = Start-Process @launch
		$reached = $false
		try {
			while ($timer.Elapsed.TotalSeconds -lt $TimeoutSeconds) {
				$child.Refresh()
				if ($child.HasExited) { break }
				Start-Sleep -Milliseconds 100
				if ($AcknowledgeDriverWarning) {
					& $Capture $child.Id --ack-driver-warning
				}
				if (!$CaptureStdout) { & $Capture $child.Id $log }
				if ((Test-Path -LiteralPath $log) -and
					(Get-Content -LiteralPath $log -Raw) -match $Endpoint) {
					$reached = $true
					break
				}
			}
			$timer.Stop()
			if ($reached) { Start-Sleep -Milliseconds 1000 }
			$child.Refresh()
			$phaseSeconds = $null
			if ($reached -and $PhaseStart) {
				$phaseTime = $null
				foreach ($line in Get-Content -LiteralPath $log) {
					if ($line -notmatch '^\[([^]]+)\]') { continue }
					$stamp = [datetime]::Parse($Matches[1])
					if ($line -match $PhaseStart) { $phaseTime = $stamp }
					if (($line -match $Endpoint) -and $phaseTime) {
						$phaseSeconds = ($stamp - $phaseTime).TotalSeconds
						break
					}
				}
			}
			$sample = [ordered]@{
				variant = $variant
				run = $run
				warmup = ($run -eq 0)
				executable = $executable
				sha256 = (Get-FileHash -LiteralPath $executable).Hash
				data = $Data
				arguments = $Arguments
				capture_stdout = [bool]$CaptureStdout
				acknowledge_driver_warning = [bool]$AcknowledgeDriverWarning
				endpoint = $Endpoint
				seconds = $timer.Elapsed.TotalSeconds
				phase_start = $PhaseStart
				phase_seconds = $phaseSeconds
				reached_endpoint = $reached
				exit_code = if ($child.HasExited) { $child.ExitCode } else { $null }
				private_bytes = if ($child.HasExited) { $null } else {
					$child.PrivateMemorySize64
				}
				peak_private_bytes = if ($child.HasExited) { $null } else {
					$child.PeakPagedMemorySize64
				}
				working_set = if ($child.HasExited) { $null } else {
					$child.WorkingSet64
				}
				peak_working_set = if ($child.HasExited) { $null } else {
					$child.PeakWorkingSet64
				}
				log = $log
			}
			$observations += [PSCustomObject]$sample
			$observations | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath `
				(Join-Path $outputPath 'results.json')
			Write-Output ($sample | ConvertTo-Json -Compress)
		} finally {
			$child.Refresh()
			if (!$child.HasExited) { Stop-Process -Id $child.Id -Force }
			$child.Dispose()
		}
	}
}
