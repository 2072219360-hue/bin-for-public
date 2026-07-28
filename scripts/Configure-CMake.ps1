[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$QtRoot,
    [Parameter(Mandatory = $true)]
    [string]$VtkRoot
)

$ErrorActionPreference = "Stop"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
. (Join-Path $PSScriptRoot "Local-Config.ps1")
Import-Dev701VsEnvironment

& (Join-Path $PSScriptRoot "Check-Environment.ps1") -QtRoot $QtRoot -VtkRoot $VtkRoot
if ($LASTEXITCODE -ne 0) { throw "Environment check failed." }

$qtConfig = Join-Path $QtRoot "lib\cmake\Qt5"
$vtkConfig = Get-ChildItem $VtkRoot -Filter VTKConfig.cmake -Recurse | Select-Object -First 1
if (-not $vtkConfig) { throw "VTKConfig.cmake was not found below $VtkRoot" }

Push-Location $repoRoot
try {
    $buildDir = Join-Path $repoRoot "out\\build\\vs2017-x64-debug"
    & cmake -S $repoRoot -B $buildDir -G Ninja `
        "-DCMAKE_BUILD_TYPE=Debug" `
        "-DQt5_DIR=$qtConfig" `
        "-DVTK_DIR=$($vtkConfig.Directory.FullName)"
    if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed." }
} finally {
    Pop-Location
}
