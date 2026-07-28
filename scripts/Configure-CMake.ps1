[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$QtRoot,
    [Parameter(Mandatory = $true)]
    [string]$VtkRoot
)

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "Vs2017-Environment.ps1")
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path

& (Join-Path $PSScriptRoot "Check-Environment.ps1") -QtRoot $QtRoot -VtkRoot $VtkRoot
if ($LASTEXITCODE -ne 0) { throw "Environment check failed." }
$null = Enter-Dev701Vs2017Environment

$qtConfig = Join-Path $QtRoot "lib\cmake\Qt5"
$vtkConfig = Get-Item (Join-Path $VtkRoot "lib\cmake\vtk-8.2\VTKConfig.cmake") -ErrorAction SilentlyContinue
if (-not $vtkConfig) {
    $vtkConfig = Get-ChildItem $VtkRoot -Filter VTKConfig.cmake -Recurse -ErrorAction SilentlyContinue |
        Select-Object -First 1
}
if (-not $vtkConfig) { throw "VTKConfig.cmake was not found below $VtkRoot" }

Push-Location $repoRoot
try {
    $buildDir = Join-Path $repoRoot "out\\build\\vs2017-x64-debug"
    $cacheFile = Join-Path $buildDir "CMakeCache.txt"
    if (Test-Path $cacheFile) {
        $cachedGenerator = Get-Content $cacheFile | Where-Object {
            $_ -like "CMAKE_GENERATOR:INTERNAL=*"
        } | Select-Object -First 1
        if ($cachedGenerator -and $cachedGenerator -ne "CMAKE_GENERATOR:INTERNAL=Ninja") {
            throw "Build directory uses another generator ($cachedGenerator). Run build.ps1 -Clean."
        }
    }
    & cmake -S $repoRoot -B $buildDir -G Ninja `
        "-DCMAKE_BUILD_TYPE=Debug" `
        "-DQt5_DIR=$qtConfig" `
        "-DVTK_DIR=$($vtkConfig.Directory.FullName)"
    if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed." }
    $configuredCompiler = Get-Content $cacheFile | Where-Object {
        $_ -match '^CMAKE_CXX_COMPILER:(FILEPATH|STRING)='
    } | Select-Object -First 1
    if (-not $configuredCompiler) { throw "CMake did not record a C++ compiler in $cacheFile" }
    $configuredCompilerPath = ($configuredCompiler -split '=', 2)[1].Replace('/', '\')
    $expectedCompilerPath = (Get-Command cl.exe).Source.Replace('/', '\')
    if (-not [IO.Path]::GetFullPath($configuredCompilerPath).Equals(
        [IO.Path]::GetFullPath($expectedCompilerPath), [StringComparison]::OrdinalIgnoreCase)) {
        throw "CMake selected an unexpected compiler: $configuredCompilerPath (expected: $expectedCompilerPath). Run build.ps1 -Clean."
    }
} finally {
    Pop-Location
}
