[CmdletBinding()]
param()

$ErrorActionPreference = "Stop"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$runtimeRoot = Join-Path $repoRoot "out\runtime\Debug"
$exe = Join-Path $runtimeRoot "Dev701QtNSPlatformBase.exe"
if (-not (Test-Path $exe)) {
    throw "Deployed executable not found. Run .\scripts\build.ps1 first: $exe"
}

Write-Host "Starting deployed runtime: $runtimeRoot"
Start-Process -FilePath $exe -WorkingDirectory $runtimeRoot
