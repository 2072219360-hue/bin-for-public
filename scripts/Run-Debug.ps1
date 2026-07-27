[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$QtRoot,
    [Parameter(Mandatory = $true)]
    [string]$VtkRoot
)

$ErrorActionPreference = "Stop"
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$exe = Join-Path $repoRoot "build\\Debug-x64\\Dev701QtNSPlatformBase.exe"
if (-not (Test-Path $exe)) { throw "Executable was not found. Run Build-Debug.ps1 first." }

$runtimePaths = @(
    (Join-Path $QtRoot "bin"),
    (Join-Path $VtkRoot "bin"),
    (Join-Path $repoRoot "src\\qwt\\lib"),
    (Join-Path $repoRoot "src\\qwtplot3d\\lib"),
    (Join-Path $repoRoot "src\\QGLViewer\\lib"),
    (Join-Path $repoRoot "Qssh-master\\lib"),
    $repoRoot
) | Where-Object { Test-Path $_ }

$env:PATH = (($runtimePaths -join ";") + ";" + $env:PATH)
Start-Process -FilePath $exe -WorkingDirectory $repoRoot
