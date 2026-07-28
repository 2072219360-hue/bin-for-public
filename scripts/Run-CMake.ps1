[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$QtRoot,
    [Parameter(Mandatory = $true)]
    [string]$VtkRoot,
    [string[]]$AdditionalRuntimePath = @()
)

$ErrorActionPreference = "Stop"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$runtimeRoot = Join-Path $repoRoot "out\runtime\Debug"
$exe = Join-Path $runtimeRoot "Dev701QtNSPlatformBase.exe"
if (-not (Test-Path $exe)) { throw "Executable not found. Run Build-CMake.ps1 first: $exe" }

$runtimePaths = @(
    (Join-Path $QtRoot "bin"),
    (Join-Path $VtkRoot "bin")
) + $AdditionalRuntimePath
$runtimePaths = $runtimePaths | Where-Object { Test-Path $_ }
$env:PATH = (($runtimePaths -join ";") + ";" + $env:PATH)

Write-Host "Starting from isolated runtime directory: $runtimeRoot"
Start-Process -FilePath $exe -WorkingDirectory $runtimeRoot
