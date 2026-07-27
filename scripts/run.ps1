[CmdletBinding()]
param()

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "Local-Config.ps1")
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$runtimeRoot = Join-Path $repoRoot "out\runtime\Debug"
$exe = Join-Path $runtimeRoot "Dev701QtNSPlatformBase.exe"
if (-not (Test-Path $exe)) {
    throw "Deployed executable not found. Run .\scripts\build.ps1 first: $exe"
}
$missingRuntimeDlls = @(Get-Dev701MissingRuntimeDlls @($runtimeRoot))
if ($missingRuntimeDlls.Count -gt 0) {
    throw "Runtime is incomplete; missing x64 DLLs: $($missingRuntimeDlls -join ', '). Run .\scripts\build.ps1."
}
$qwindows = Join-Path $runtimeRoot "platforms\qwindows.dll"
if (-not (Test-Dev701PeX64 $qwindows)) {
    throw "Qt platform plugin is missing or not x64: $qwindows. Run .\scripts\build.ps1."
}

Write-Host "Starting deployed runtime: $runtimeRoot"
Start-Process -FilePath $exe -WorkingDirectory $runtimeRoot
