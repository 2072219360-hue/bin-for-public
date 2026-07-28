[CmdletBinding()]
param()

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "Vs2017-Environment.ps1")
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$null = Enter-Dev701Vs2017Environment
Push-Location $repoRoot
try {
    & cmake --build (Join-Path $repoRoot "out\\build\\vs2017-x64-debug") --config Debug --parallel 1
    if ($LASTEXITCODE -ne 0) { throw "CMake build failed." }
} finally {
    Pop-Location
}
