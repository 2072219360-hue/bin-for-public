[CmdletBinding()]
param()

$ErrorActionPreference = "Stop"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
. (Join-Path $PSScriptRoot "Local-Config.ps1")
Import-Dev701VsEnvironment
Push-Location $repoRoot
try {
    & cmake --build (Join-Path $repoRoot "out\\build\\vs2017-x64-debug") --config Debug --parallel 1
    if ($LASTEXITCODE -ne 0) { throw "CMake build failed." }
} finally {
    Pop-Location
}
