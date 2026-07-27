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
    $required = @("qwtd.dll", "qwtplot3d.dll", "QGLViewerd2.dll", "QSshd.dll")
    return @($required | Where-Object {
        $name = $_
        -not ($Directories | Where-Object {
            Test-Dev701PeX64 (Join-Path $_ $name)
        } | Select-Object -First 1)
    })
}
