[CmdletBinding()]
param(
    [string]$QtRoot,
    [string]$VtkRoot,
    [string[]]$AdditionalRuntimePath = @(),
    [switch]$NonInteractive
)

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "Local-Config.ps1")
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$existingConfig = $null
if (Test-Path $script:Dev701ConfigFile) {
    $existingConfig = Get-Content $script:Dev701ConfigFile -Raw | ConvertFrom-Json
}

function Find-QtRoot {
    $candidates = @($existingConfig.qtRoot, $env:QTDIR, $env:QT_ROOT)
    $qmake = Get-Command qmake.exe -ErrorAction SilentlyContinue
    if ($qmake) { $candidates += Split-Path -Parent (Split-Path -Parent $qmake.Source) }
    foreach ($drive in Get-PSDrive -PSProvider FileSystem) {
        $candidates += Join-Path $drive.Root "Qt\5.14.1\msvc2017_64"
    }
    foreach ($candidate in $candidates | Where-Object { $_ } | Select-Object -Unique) {
        if (Test-Path (Join-Path $candidate "bin\qmake.exe")) {
            $version = & (Join-Path $candidate "bin\qmake.exe") -query QT_VERSION
            if ($version -eq "5.14.1") { return (Resolve-Path $candidate).Path }
        }
    }
    return $null
}

function Find-VtkRoot {
    $candidates = @($existingConfig.vtkRoot, $env:VTK_ROOT, $env:VTK_DIR)
    foreach ($drive in Get-PSDrive -PSProvider FileSystem) {
        $candidates += Join-Path $drive.Root "deps\VTK-8.2"
        $candidates += Join-Path $drive.Root "VTK-8.2"
    }
    foreach ($candidate in $candidates | Where-Object { $_ } | Select-Object -Unique) {
        if (Test-Path $candidate -PathType Leaf) { $candidate = Split-Path -Parent $candidate }
        $current = $candidate
        while ($current -and -not (Test-Path (Join-Path $current "include\vtk-8.2\vtkVersion.h"))) {
            $parent = Split-Path -Parent $current
            if ($parent -eq $current) { break }
            $current = $parent
        }
        if ($current -and (Test-Path (Join-Path $current "include\vtk-8.2\vtkVersion.h"))) {
            return (Resolve-Path $current).Path
        }
    }
    return $null
}

function Find-RuntimePaths {
    $candidates = @($existingConfig.additionalRuntimePath, $env:DEV701_RUNTIME_ROOT)
    $candidates += @(
        (Join-Path $repoRoot "binForPublic"),
        (Join-Path $repoRoot "deps\\runtime\\Debug"),
        (Join-Path $repoRoot "runtime"),
        (Join-Path $repoRoot "lib")
    )
    $valid = @()
    foreach ($candidate in $candidates | Where-Object { $_ } | Select-Object -Unique) {
        if (Test-Path $candidate -PathType Container) {
            $candidate = (Resolve-Path $candidate).Path
            $hasRequiredDll = @("qwtd.dll", "qwtplot3d.dll", "QGLViewerd2.dll", "QSshd.dll", "jsoncpp.dll") |
                Where-Object { Test-Dev701PeX64 (Join-Path $candidate $_) } |
                Select-Object -First 1
            if ($hasRequiredDll) { $valid += $candidate }
        }
    }
    return @($valid | Select-Object -Unique)
}

if (-not $QtRoot) { $QtRoot = Find-QtRoot }
if (-not $VtkRoot) { $VtkRoot = Find-VtkRoot }
if ($AdditionalRuntimePath.Count -eq 0) { $AdditionalRuntimePath = @(Find-RuntimePaths) }

if (-not $QtRoot -and -not $NonInteractive) { $QtRoot = Read-Host "Qt 5.14.1 msvc2017_64 root" }
if (-not $VtkRoot -and -not $NonInteractive) { $VtkRoot = Read-Host "VTK 8.2 install root" }
if (-not $QtRoot) { throw "Qt was not found. Re-run setup.ps1 with -QtRoot <path>." }
if (-not $VtkRoot) { throw "VTK was not found. Re-run setup.ps1 with -VtkRoot <path>." }

$QtRoot = Resolve-ExistingDirectory $QtRoot "Qt"
$VtkRoot = Resolve-ExistingDirectory $VtkRoot "VTK"
$AdditionalRuntimePath = @($AdditionalRuntimePath | Where-Object { $_ } | ForEach-Object {
    Resolve-ExistingDirectory $_ "Additional runtime"
})

$missingRuntimeDlls = @(Get-Dev701MissingRuntimeDlls $AdditionalRuntimePath)
if ($missingRuntimeDlls.Count -gt 0 -and -not $NonInteractive) {
    Write-Host "Missing verified x64 runtime DLLs: $($missingRuntimeDlls -join ', ')" -ForegroundColor Yellow
    $answer = Read-Host "Directory/directories containing them (separate with semicolons)"
    if ($answer) {
        $AdditionalRuntimePath += @($answer -split ";" | Where-Object { $_ } | ForEach-Object {
            Resolve-ExistingDirectory ($_.Trim()) "Additional runtime"
        })
        $AdditionalRuntimePath = @($AdditionalRuntimePath | Select-Object -Unique)
    }
    $missingRuntimeDlls = @(Get-Dev701MissingRuntimeDlls $AdditionalRuntimePath)
}
if ($missingRuntimeDlls.Count -gt 0) {
    throw "Missing x64 Debug runtime DLLs: $($missingRuntimeDlls -join ', '). Use -AdditionalRuntimePath."
}

& (Join-Path $PSScriptRoot "Check-Environment.ps1") -QtRoot $QtRoot -VtkRoot $VtkRoot

$config = [ordered]@{
    schemaVersion = 1
    qtRoot = $QtRoot
    vtkRoot = $VtkRoot
    additionalRuntimePath = $AdditionalRuntimePath
}
$config | ConvertTo-Json -Depth 3 | Set-Content $script:Dev701ConfigFile -Encoding UTF8

Write-Host "Local configuration saved: $script:Dev701ConfigFile" -ForegroundColor Green
Write-Host "Next commands:"
Write-Host "  .\scripts\build.ps1"
Write-Host "  .\scripts\run.ps1"
