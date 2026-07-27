[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string]$QtRoot,
    [string]$Python = "python"
)

$ErrorActionPreference = "Stop"

if (Test-Path (Join-Path $QtRoot "bin\\qmake.exe")) {
    Write-Host "Compatible Qt installation already exists: $QtRoot"
    exit 0
}

$qtParent = Split-Path -Parent $QtRoot
New-Item -ItemType Directory -Force $qtParent | Out-Null

& $Python -m pip install --user "aqtinstall==2.1.0"
if ($LASTEXITCODE -ne 0) { throw "Unable to install aqtinstall." }

# Qt 5.14.1 must match the MSVC toolset used by the project and VTK.
& $Python -m aqt install-qt windows desktop 5.14.1 win64_msvc2017_64 `
    -O $qtParent -m qtcharts
if ($LASTEXITCODE -ne 0) { throw "Qt download or installation failed." }

if (-not (Test-Path (Join-Path $QtRoot "bin\\qmake.exe"))) {
    throw "Qt was installed, but qmake.exe was not found at $QtRoot."
}

Write-Host "Qt is ready: $QtRoot"
