[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$QtRoot,
    [Parameter(Mandatory = $true)]
    [string]$VtkRoot,
    [string]$WindowsSdkVersion = ""
)

$ErrorActionPreference = "Stop"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$project = Join-Path $repoRoot "Dev701QtNSPlatformBase.vcxproj"
$qtMsBuild = Join-Path $repoRoot "QtMsBuild"
$msbuild = "${env:ProgramFiles(x86)}\\Microsoft Visual Studio\\2017\\Community\\MSBuild\\15.0\\Bin\\amd64\\MSBuild.exe"

if (-not (Test-Path $msbuild)) { throw "Visual Studio 2017 MSBuild was not found: $msbuild" }
if (-not (Test-Path (Join-Path $QtRoot "bin\\qmake.exe"))) { throw "Invalid Qt root: $QtRoot" }
if (-not (Test-Path (Join-Path $QtRoot "lib\\Qt5Chartsd.lib"))) { throw "Qt Charts debug library is missing." }
if (-not (Test-Path (Join-Path $VtkRoot "include\\vtk-8.2"))) { throw "Invalid VTK root: $VtkRoot" }
if (-not (Test-Path (Join-Path $VtkRoot "lib\\vtkCommonCore-8.2.lib"))) { throw "VTK Debug libraries are missing." }

function Ensure-Junction([string]$Link, [string]$Target) {
    if (Test-Path $Link) {
        $item = Get-Item -Force $Link
        $linkPath = [IO.Path]::GetFullPath($Link).TrimEnd('\\')
        $targetPath = [IO.Path]::GetFullPath($Target).TrimEnd('\\')
        $junctionTargets = @($item.Target) | ForEach-Object {
            [IO.Path]::GetFullPath($_).TrimEnd('\\')
        }
        if (($linkPath -eq $targetPath) -or ($junctionTargets -contains $targetPath)) {
            return
        }
        throw "Required path already exists but points elsewhere: $Link"
    }
    New-Item -ItemType Directory -Force (Split-Path -Parent $Link) | Out-Null
    New-Item -ItemType Junction -Path $Link -Target $Target | Out-Null
}

# The checked-in project has legacy E: absolute paths. Never replace user paths.
if (-not (Test-Path "E:\\")) { subst E: (Split-Path -Qualifier $repoRoot) }
Ensure-Junction "E:\\Qt\\5.14.1\\msvc2017_64" $QtRoot
Ensure-Junction "E:\\Project20260602\\Dev701QtNSPlatformBase" $repoRoot
Ensure-Junction (Join-Path $repoRoot "VTK") $VtkRoot

if ([string]::IsNullOrWhiteSpace($WindowsSdkVersion)) {
    $sdkRoot = "${env:ProgramFiles(x86)}\\Windows Kits\\10\\Lib"
    $WindowsSdkVersion = (Get-ChildItem $sdkRoot -Directory | Sort-Object Name -Descending |
        Select-Object -First 1 -ExpandProperty Name)
}
if ([string]::IsNullOrWhiteSpace($WindowsSdkVersion)) { throw "No Windows 10 SDK was found." }

$outDir = Join-Path $repoRoot "build\\Debug-x64\\"
$intDir = Join-Path $repoRoot "build\\obj\\Debug-x64\\"
$common = "/p:QtMsBuild=$qtMsBuild;Configuration=Debug;Platform=x64;WindowsTargetPlatformVersion=$WindowsSdkVersion;OutDir=$outDir;IntDir=$intDir"

# Qt/MSBuild writes qmake metadata during Build. Clean removes this directory.
& $msbuild $project /t:Clean /m:1 /verbosity:minimal $common
if ($LASTEXITCODE -ne 0) { throw "Project clean failed." }
New-Item -ItemType Directory -Force (Join-Path $intDir "qt\\qmake") | Out-Null
# The legacy project directs every compiler instance to vc141.pdb. Build
# serially so MSVC does not fail with C1041 on concurrent PDB writes.
& $msbuild $project /t:Build /m:1 /verbosity:minimal $common
if ($LASTEXITCODE -ne 0) { throw "Project build failed." }

$exe = Join-Path $outDir "Dev701QtNSPlatformBase.exe"
if (-not (Test-Path $exe)) { throw "Build succeeded without the expected executable: $exe" }
Write-Host "Build complete: $exe"
