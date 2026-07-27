[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$QtRoot,
    [Parameter(Mandatory = $true)]
    [string]$VtkRoot,
    [string]$SourceRoot = (Join-Path $env:LOCALAPPDATA "Dev701Deps\\vtk-8.2.0"),
    [string]$CMake = "cmake"
)

$ErrorActionPreference = "Stop"
$qmake = Join-Path $QtRoot "bin\\qmake.exe"
if (-not (Test-Path $qmake)) { throw "Qt qmake.exe was not found: $qmake" }
if (Test-Path (Join-Path $VtkRoot "lib\\vtkCommonCore-8.2.lib")) {
    Write-Host "VTK installation already exists: $VtkRoot"
    exit 0
}

if (-not (Test-Path (Join-Path $SourceRoot ".git"))) {
    New-Item -ItemType Directory -Force (Split-Path -Parent $SourceRoot) | Out-Null
    & git clone --branch v8.2.0 --depth 1 https://gitlab.kitware.com/vtk/vtk.git $SourceRoot
    if ($LASTEXITCODE -ne 0) { throw "Unable to clone VTK v8.2.0." }
}

$buildRoot = "$SourceRoot-build-vs2017-x64"
& $CMake -S $SourceRoot -B $buildRoot -G "Visual Studio 15 2017" -A x64 `
    "-DCMAKE_INSTALL_PREFIX=$VtkRoot" `
    "-DVTK_GROUP_ENABLE_Qt=YES" `
    "-DVTK_QT_VERSION=5" `
    "-DQt5_DIR=$(Join-Path $QtRoot 'lib\\cmake\\Qt5')" `
    "-DBUILD_SHARED_LIBS=ON"
if ($LASTEXITCODE -ne 0) { throw "VTK configuration failed." }

& $CMake --build $buildRoot --config Debug --target INSTALL -- /m
if ($LASTEXITCODE -ne 0) { throw "VTK Debug build/install failed." }

if (-not (Test-Path (Join-Path $VtkRoot "lib\\vtkCommonCore-8.2.lib"))) {
    throw "VTK build completed without the expected library."
}

Write-Host "VTK is ready: $VtkRoot"
