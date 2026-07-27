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

$dllRoots = @((Join-Path $VtkRoot "bin")) + $AdditionalRuntimePath
$originalPath = $env:PATH
$env:PATH = (($dllRoots -join ";") + ";" + $originalPath)
try {
    & $windeployqt --debug --no-translations --dir $runtimeRoot $exe
    if ($LASTEXITCODE -ne 0) { throw "windeployqt failed." }
} finally {
    $env:PATH = $originalPath
}
foreach ($root in $dllRoots | Where-Object { Test-Path $_ -PathType Container }) {
    Get-ChildItem $root -Filter *.dll -File | ForEach-Object {
        Copy-Item $_.FullName $runtimeRoot -Force
    }
}
$missingRuntimeDlls = @(Get-Dev701MissingRuntimeDlls @($runtimeRoot))
if ($missingRuntimeDlls.Count -gt 0) {
    throw "Deployment is incomplete; missing x64 DLLs: $($missingRuntimeDlls -join ', ')"
}
Write-Host "Runtime deployed: $runtimeRoot" -ForegroundColor Green
