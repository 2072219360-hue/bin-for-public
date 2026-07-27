[CmdletBinding()]
param()

$ErrorActionPreference = "Stop"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
Push-Location $repoRoot
try {
    & cmake --build --preset vs2017-x64-debug
    if ($LASTEXITCODE -ne 0) { throw "CMake build failed." }
} finally {
    Pop-Location
}
