$script:Dev701ConfigFile = Join-Path (Resolve-Path (Join-Path $PSScriptRoot "..")) ".dev701.local.json"

function Get-Dev701LocalConfig {
    if (-not (Test-Path $script:Dev701ConfigFile)) {
        throw "Local configuration not found. Run .\scripts\setup.ps1 first."
    }
    return Get-Content $script:Dev701ConfigFile -Raw | ConvertFrom-Json
}

function Resolve-ExistingDirectory([string]$Path, [string]$Name) {
    if ([string]::IsNullOrWhiteSpace($Path) -or -not (Test-Path $Path -PathType Container)) {
        throw "$Name directory does not exist: $Path"
    }
    return (Resolve-Path $Path).Path
}

function Import-Dev701VsEnvironment {
    [CmdletBinding()]
    param()

    $vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\\Installer\\vswhere.exe"
    if (-not (Test-Path $vswhere)) {
        throw "Visual Studio locator was not found: $vswhere"
    }

    $installPath = (& $vswhere -latest -products * -version "[15.0,16.0)" `
        -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
        -property installationPath | Select-Object -First 1).Trim()
    if ([string]::IsNullOrWhiteSpace($installPath)) {
        throw "Visual Studio 2017 C++ tools were not found."
    }

    $vcvars = Join-Path $installPath "VC\\Auxiliary\\Build\\vcvars64.bat"
    if (-not (Test-Path $vcvars)) {
        throw "VS2017 x64 environment script was not found: $vcvars"
    }

    $environmentLines = & $env:ComSpec /d /s /c ('call "{0}" >nul && set' -f $vcvars)
    if ($LASTEXITCODE -ne 0) {
        throw "Could not initialize the VS2017 x64 environment."
    }

    foreach ($line in $environmentLines) {
        if ($line -match '^([^=]+)=(.*)$') {
            [Environment]::SetEnvironmentVariable($matches[1], $matches[2], "Process")
        }
    }

    if ([string]::IsNullOrWhiteSpace($env:INCLUDE) -or [string]::IsNullOrWhiteSpace($env:LIB)) {
        throw "VS2017 x64 environment is incomplete: INCLUDE or LIB is not set."
    }
}

function Test-Dev701PeX64([string]$Path) {
    if (-not (Test-Path $Path -PathType Leaf)) { return $false }
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

function Get-Dev701MissingRuntimeDlls([string[]]$Directories) {
    # Qwt, QwtPlot3D, and QGLViewer are linked from static libraries in the
    # known Debug target. Validate only the DLLs imported by the executable.
    $required = @("QSshd.dll", "jsoncpp.dll")
    return @($required | Where-Object {
        $name = $_
        -not ($Directories | Where-Object {
            Test-Dev701PeX64 (Join-Path $_ $name)
        } | Select-Object -First 1)
    })
}
