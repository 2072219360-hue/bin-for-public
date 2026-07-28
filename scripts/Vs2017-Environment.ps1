function Find-Dev701Vs2017Root {
    $vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $root = & $vswhere -version "[15.0,16.0)" -products * `
            -requires Microsoft.VisualStudio.Component.VC.v141.x86.x64 `
            -property installationPath | Select-Object -First 1
        if (-not [string]::IsNullOrWhiteSpace($root)) { return $root }
    }

    foreach ($edition in @("Community", "Professional", "Enterprise", "BuildTools")) {
        $root = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\2017\$edition"
        if (Test-Path (Join-Path $root "VC\Auxiliary\Build\vcvars64.bat")) { return $root }
    }
    return $null
}

function Enter-Dev701Vs2017Environment {
    $existingCompiler = Get-Command cl.exe -ErrorAction SilentlyContinue
    $existingStringHeader = $env:INCLUDE -split ';' | Where-Object { $_ } | ForEach-Object {
        Join-Path $_ "string"
    } | Where-Object { Test-Path $_ -PathType Leaf } | Select-Object -First 1
    $existingWindowsHeader = $env:INCLUDE -split ';' | Where-Object { $_ } | ForEach-Object {
        Join-Path $_ "windows.h"
    } | Where-Object { Test-Path $_ -PathType Leaf } | Select-Object -First 1
    if ($env:VisualStudioVersion -eq "15.0" -and $existingCompiler -and
        -not [string]::IsNullOrWhiteSpace($env:LIB) -and $existingStringHeader -and $existingWindowsHeader) {
        return [PSCustomObject]@{
            Root = $env:VSINSTALLDIR.TrimEnd('\')
            Compiler = $existingCompiler.Source
            StringHeader = $existingStringHeader
            WindowsHeader = $existingWindowsHeader
        }
    }

    $vsRoot = Find-Dev701Vs2017Root
    if (-not $vsRoot) {
        throw "Visual Studio 2017 with the MSVC v141 x64 toolset was not found."
    }

    $vcvars = Join-Path $vsRoot "VC\Auxiliary\Build\vcvars64.bat"
    if (-not (Test-Path $vcvars)) { throw "vcvars64.bat was not found: $vcvars" }

    $command = "call `"$vcvars`" >nul && set"
    $environment = & $env:COMSPEC /d /s /c $command
    if ($LASTEXITCODE -ne 0) { throw "Unable to initialize the VS2017 x64 developer environment." }

    foreach ($line in $environment) {
        if ($line -match '^([^=]+)=(.*)$') {
            Set-Item -Path "Env:$($matches[1])" -Value $matches[2]
        }
    }

    $compiler = Get-Command cl.exe -ErrorAction SilentlyContinue
    if (-not $compiler) { throw "VS2017 environment loaded, but cl.exe is not on PATH." }
    if ([string]::IsNullOrWhiteSpace($env:INCLUDE)) { throw "VS2017 INCLUDE is empty." }
    if ([string]::IsNullOrWhiteSpace($env:LIB)) { throw "VS2017 LIB is empty." }

    $stringHeader = $env:INCLUDE -split ';' | Where-Object { $_ } | ForEach-Object {
        Join-Path $_ "string"
    } | Where-Object { Test-Path $_ -PathType Leaf } | Select-Object -First 1
    if (-not $stringHeader) {
        throw "The C++ standard library header <string> is not reachable through VS2017 INCLUDE."
    }

    $windowsHeader = $env:INCLUDE -split ';' | Where-Object { $_ } | ForEach-Object {
        Join-Path $_ "windows.h"
    } | Where-Object { Test-Path $_ -PathType Leaf } | Select-Object -First 1
    if (-not $windowsHeader) {
        throw "The Windows SDK header <windows.h> is not reachable through VS2017 INCLUDE."
    }

    return [PSCustomObject]@{
        Root = $vsRoot
        Compiler = $compiler.Source
        StringHeader = $stringHeader
        WindowsHeader = $windowsHeader
    }
}

function Test-Dev701Vs2017Toolchain {
    $tempRoot = Join-Path ([IO.Path]::GetTempPath()) ("dev701-toolchain-" + [guid]::NewGuid().ToString("N"))
    New-Item -ItemType Directory -Force $tempRoot | Out-Null
    $source = Join-Path $tempRoot "probe.cpp"
    $program = Join-Path $tempRoot "probe.exe"
    @"
#include <string>
#include <windows.h>
int main() { std::string value = "dev701"; return value.empty() ? 1 : 0; }
"@ | Set-Content $source -Encoding ASCII

    Push-Location $tempRoot
    try {
        $output = & cl.exe /nologo /EHsc /MDd $source "/Fe:$program" 2>&1
        if ($LASTEXITCODE -ne 0 -or -not (Test-Path $program)) {
            throw "VS2017 clean compile/link probe failed:`n$($output -join [Environment]::NewLine)"
        }
        & $program
        if ($LASTEXITCODE -ne 0) { throw "VS2017 toolchain probe executable failed with exit code $LASTEXITCODE." }
    } finally {
        Pop-Location
        Remove-Item $tempRoot -Recurse -Force -ErrorAction SilentlyContinue
    }
}
