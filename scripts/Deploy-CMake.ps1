[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$QtRoot,
    [Parameter(Mandatory = $true)]
    [string]$VtkRoot,
    [string[]]$AdditionalRuntimePath = @()
)

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "Local-Config.ps1")
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$runtimeRoot = Join-Path $repoRoot "out\runtime\Debug"
$exe = Join-Path $runtimeRoot "Dev701QtNSPlatformBase.exe"
if (-not (Test-Path $exe)) { throw "Executable not found: $exe" }

$windeployqt = Join-Path $QtRoot "bin\windeployqt.exe"
if (-not (Test-Path $windeployqt)) { throw "windeployqt.exe not found: $windeployqt" }

$vtkBin = Join-Path $VtkRoot "bin"
$dllRoots = @($vtkBin) + $AdditionalRuntimePath
$originalPath = $env:PATH
$env:PATH = (($dllRoots -join ";") + ";" + $originalPath)
try {
    & $windeployqt --debug --no-translations --dir $runtimeRoot $exe
    if ($LASTEXITCODE -ne 0) { throw "windeployqt failed." }
} finally {
    $env:PATH = $originalPath
}
if (Test-Path $vtkBin -PathType Container) {
    Get-ChildItem $vtkBin -Filter *.dll -File | ForEach-Object {
        Copy-Item $_.FullName $runtimeRoot -Force
    }
}

$projectDlls = @("qwtd.dll", "qwtplot3d.dll", "QGLViewerd2.dll", "QSshd.dll", "jsoncpp.dll")
$runtimeDataDirectories = @("help", "newtemplate", "mpisupport", "pyengine", "pltDemo", "ui")
foreach ($root in $AdditionalRuntimePath | Where-Object { Test-Path $_ -PathType Container }) {
    foreach ($name in $projectDlls) {
        $source = Join-Path $root $name
        if (Test-Dev701PeX64 $source) {
            Copy-Item $source $runtimeRoot -Force
        }
    }
    foreach ($name in $runtimeDataDirectories) {
        $source = Join-Path $root $name
        if (Test-Path $source -PathType Container) {
            $destination = Join-Path $runtimeRoot $name
            New-Item -ItemType Directory -Force $destination | Out-Null
            Get-ChildItem $source -Force | ForEach-Object {
                Copy-Item $_.FullName $destination -Recurse -Force
            }
        }
    }
}

New-Item -ItemType Directory -Force (Join-Path $runtimeRoot "logs") | Out-Null
New-Item -ItemType Directory -Force (Join-Path $runtimeRoot "workSpace") | Out-Null
$missingRuntimeDlls = @(Get-Dev701MissingRuntimeDlls @($runtimeRoot))
if ($missingRuntimeDlls.Count -gt 0) {
    throw "Deployment is incomplete; missing x64 DLLs: $($missingRuntimeDlls -join ', ')"
}
$qwindows = Join-Path $runtimeRoot "platforms\qwindows.dll"
if (-not (Test-Dev701PeX64 $qwindows)) {
    throw "Deployment is incomplete; Qt x64 platform plugin is missing: $qwindows"
}
Write-Host "Runtime deployed: $runtimeRoot" -ForegroundColor Green
