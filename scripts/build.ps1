[CmdletBinding()]
param()

$ErrorActionPreference = "Stop"
. (Join-Path $PSScriptRoot "Local-Config.ps1")
$config = Get-Dev701LocalConfig

& (Join-Path $PSScriptRoot "Configure-CMake.ps1") `
    -QtRoot $config.qtRoot -VtkRoot $config.vtkRoot
& (Join-Path $PSScriptRoot "Build-CMake.ps1")
& (Join-Path $PSScriptRoot "Deploy-CMake.ps1") `
    -QtRoot $config.qtRoot `
    -VtkRoot $config.vtkRoot `
    -AdditionalRuntimePath @($config.additionalRuntimePath)
