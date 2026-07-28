[CmdletBinding()]
param(
    [switch]$Clean
)

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "Local-Config.ps1")
$config = Get-Dev701LocalConfig
$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$buildRoot = Join-Path $repoRoot "out\build\vs2017-x64-debug"
$runtimeRoot = Join-Path $repoRoot "out\runtime\Debug"
if ($Clean) {
    Write-Host "Removing generated CMake build and runtime output..."
    if (Test-Path $buildRoot) { Remove-Item $buildRoot -Recurse -Force }
    if (Test-Path $runtimeRoot) {
        Get-ChildItem $runtimeRoot -Force | Where-Object {
            $_.Name -notin @("workSpace", "logs")
        } | Remove-Item -Recurse -Force
    }
}
$runtimeExe = Join-Path $repoRoot "out\runtime\Debug\Dev701QtNSPlatformBase.exe"
if (Test-Path $runtimeExe) { Remove-Item $runtimeExe -Force }

& (Join-Path $PSScriptRoot "Configure-CMake.ps1") `
    -QtRoot $config.qtRoot -VtkRoot $config.vtkRoot
& (Join-Path $PSScriptRoot "Build-CMake.ps1")
& (Join-Path $PSScriptRoot "Deploy-CMake.ps1") `
    -QtRoot $config.qtRoot `
    -VtkRoot $config.vtkRoot `
    -AdditionalRuntimePath @($config.additionalRuntimePath)
