[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$QtRoot,
    [Parameter(Mandatory = $true)]
    [string]$VtkRoot
)

$ErrorActionPreference = "Stop"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$failed = $false

function Write-Check([bool]$Ok, [string]$Name, [string]$Detail) {
    if ($Ok) {
        Write-Host "[PASS] $Name - $Detail" -ForegroundColor Green
    } else {
        Write-Host "[FAIL] $Name - $Detail" -ForegroundColor Red
        $script:failed = $true
    }
}

function Find-VsWhere {
    $candidate = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $candidate) { return $candidate }
    return $null
}

function Find-Dumpbin([string]$VsWhere) {
    if (-not $VsWhere) { return $null }
    $vsRoot = & $VsWhere -version "[15.0,16.0)" -products * -requires Microsoft.VisualStudio.Component.VC.v141.x86.x64 -property installationPath
    if (-not $vsRoot) { return $null }
    $tools = Get-ChildItem (Join-Path $vsRoot "VC\Tools\MSVC") -Directory -ErrorAction SilentlyContinue |
        Sort-Object Name -Descending | Select-Object -First 1
    if (-not $tools) { return $null }
    $dumpbin = Join-Path $tools.FullName "bin\Hostx64\x64\dumpbin.exe"
    if (Test-Path $dumpbin) { return $dumpbin }
    return $null
}

function Test-PeX64([string]$Path) {
    if (-not (Test-Path $Path)) { return $false }
    $stream = [IO.File]::OpenRead($Path)
    try {
        $reader = New-Object IO.BinaryReader($stream)
        if ($reader.ReadUInt16() -ne 0x5A4D) { return $false }
        $stream.Position = 0x3C
        $peOffset = $reader.ReadInt32()
        $stream.Position = $peOffset
        if ($reader.ReadUInt32() -ne 0x00004550) { return $false }
        return $reader.ReadUInt16() -eq 0x8664
    } finally {
        $stream.Dispose()
    }
}

Write-Host "Dev701 conservative CMake environment check"
Write-Host "Repository: $repoRoot"

$vswhere = Find-VsWhere
Write-Check ($null -ne $vswhere) "Visual Studio locator" $(if ($vswhere) { $vswhere } else { "vswhere.exe not found" })

$vsRoot = $null
if ($vswhere) {
    $vsRoot = & $vswhere -version "[15.0,16.0)" -products * -requires Microsoft.VisualStudio.Component.VC.v141.x86.x64 -property installationPath
}
Write-Check (-not [string]::IsNullOrWhiteSpace($vsRoot)) "Visual Studio 2017 + MSVC v141 x64" $(if ($vsRoot) { $vsRoot } else { "VS2017 with the v141 x64 toolset not installed" })

$cmake = Get-Command cmake -ErrorAction SilentlyContinue
Write-Check ($null -ne $cmake) "CMake" $(if ($cmake) { (& cmake --version | Select-Object -First 1) } else { "cmake not on PATH" })
if ($cmake) {
    $cmakeHelp = (& cmake --help | Out-String)
    Write-Check ($cmakeHelp -match "Visual Studio 15 2017") "CMake VS2017 generator" "Visual Studio 15 2017"
}

$qmake = Join-Path $QtRoot "bin\qmake.exe"
$qtVersion = if (Test-Path $qmake) { (& $qmake -query QT_VERSION) } else { "missing" }
Write-Check ((Test-Path $qmake) -and $qtVersion -eq "5.14.1") "Qt 5.14.1" "$qmake (version $qtVersion)"
Write-Check (Test-Path (Join-Path $QtRoot "lib\Qt5Chartsd.lib")) "Qt Charts Debug" (Join-Path $QtRoot "lib\Qt5Chartsd.lib")
Write-Check (Test-Path (Join-Path $QtRoot "lib\cmake\Qt5\Qt5Config.cmake")) "Qt CMake package" (Join-Path $QtRoot "lib\cmake\Qt5")

$vtkConfig = Get-ChildItem $VtkRoot -Filter VTKConfig.cmake -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
Write-Check ($null -ne $vtkConfig) "VTK 8.2 CMake package" $(if ($vtkConfig) { $vtkConfig.FullName } else { "VTKConfig.cmake not found below $VtkRoot" })
Write-Check (Test-Path (Join-Path $VtkRoot "include\vtk-8.2\vtkVersion.h")) "VTK 8.2 headers" (Join-Path $VtkRoot "include\vtk-8.2")
Write-Check (Test-Path (Join-Path $VtkRoot "lib\vtkCommonCore-8.2.lib")) "VTK Debug libraries" (Join-Path $VtkRoot "lib\vtkCommonCore-8.2.lib")

$importLibraries = @(
    "lib\huangqwtd.lib",
    "lib\huangqwtplot3d.lib",
    "lib\HuangzbQGLVastyellowd2.lib",
    "lib\QSshd.lib",
    "lib\jsoncpp.lib",
    "lib\jsoncpp_static.lib"
)
foreach ($relative in $importLibraries) {
    $path = Join-Path $repoRoot $relative
    Write-Check (Test-Path $path) "Project import library" $relative
}

$dumpbin = Find-Dumpbin $vswhere
if ($dumpbin) {
    foreach ($relative in $importLibraries) {
        $path = Join-Path $repoRoot $relative
        if (Test-Path $path) {
            $headers = (& $dumpbin /headers $path 2>&1 | Out-String)
            Write-Check ($headers -match "machine \(x64\)|8664 machine") "x64 import library" $relative
        }
    }
} else {
    Write-Host "[WARN] dumpbin unavailable; import-library architecture was not checked." -ForegroundColor Yellow
}

$knownDlls = @(
    "src\qwt\lib\qwtd.dll",
    "src\qwtplot3d\lib\qwtplot3dd.dll",
    "src\QGLViewer\lib\QGLViewerd2.dll",
    "Qssh-master\lib\QSshd.dll"
)
foreach ($relative in $knownDlls) {
    $path = Join-Path $repoRoot $relative
    if (Test-Path $path) {
        if (Test-PeX64 $path) {
            Write-Host "[PASS] x64 runtime DLL - $relative (not automatically deployed)" -ForegroundColor Green
        } else {
            Write-Host "[WARN] non-x64 runtime DLL - $relative (excluded from automatic deployment)" -ForegroundColor Yellow
        }
    }
}

if ($failed) {
    throw "Environment check failed. No files or system settings were changed."
}
Write-Host "Environment check passed. No files or system settings were changed." -ForegroundColor Green
