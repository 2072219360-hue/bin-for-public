# Conservative CMake build

The CMake build is a parallel prototype. It does not edit or replace the
existing `.sln`, `.vcxproj`, `.pro`, or legacy PowerShell scripts. All generated
files go below `out/`.

## Scope

The first milestone supports only:

- Windows x64
- Visual Studio 2017 / MSVC v141
- Ninja available on `PATH`
- Debug
- Qt 5.14.1 `msvc2017_64`
- VTK 8.2.0 built against the same Qt installation

## Recommended workflow

Run setup once from PowerShell at the repository root:

```powershell
Set-ExecutionPolicy -Scope Process Bypass
.\scripts\setup.ps1
```

The script checks saved configuration, `QTDIR`/`QT_ROOT`, `VTK_ROOT`/`VTK_DIR`,
`PATH`, and conventional drive locations. If Qt or VTK cannot be found, it
asks for that path once. The result is stored in `.dev701.local.json`, which is
ignored by Git.

Setup also locates or asks once for verified x64 Debug copies of `qwtd.dll`,
`qwtplot3d.dll`, `QGLViewerd2.dll`, `QSshd.dll`, and `jsoncpp.dll`. Known 32-bit
binaries are rejected. The saved directories are reused by build and run.

Paths can also be supplied non-interactively on the first run:

```powershell
.\scripts\setup.ps1 `
  -QtRoot D:\Qt\5.14.1\msvc2017_64 `
  -VtkRoot D:\deps\VTK-8.2 `
  -AdditionalRuntimePath D:\deps\dev701-x64-debug\bin
```

After setup, daily commands have no path arguments:

```powershell
.\scripts\build.ps1
.\scripts\run.ps1
```

`build.ps1` configures, builds, and deploys Qt plugins, VTK DLLs, verified
additional dependency DLLs, and source-controlled runtime data into
`out\runtime\Debug`. `run.ps1` starts that isolated runtime.
It does not add Qt or VTK installation directories to `PATH`.

The scripts automatically load `vcvars64.bat` before invoking CMake/Ninja.
They verify `cl.exe`, `INCLUDE`, `LIB`, and the standard C++ header `<string>`,
so a clean build does not depend on a previously opened Developer PowerShell.

When an additional runtime directory contains legacy runtime-data folders
(`help`, `newtemplate`, `mpisupport`, `pyengine`, `pltDemo`, or `ui`), they are
copied as well. Missing optional folders do not prevent the main window from
starting, but the corresponding workspace, MPI, cluster, or help feature may
remain unavailable. Empty `logs` and `workSpace` directories are always made.

## Low-level commands

The parameterized scripts remain available for diagnostics and automation:

```powershell

.\scripts\Check-Environment.ps1 `
  -QtRoot D:\Qt\5.14.1\msvc2017_64 `
  -VtkRoot D:\deps\VTK-8.2

.\scripts\Configure-CMake.ps1 `
  -QtRoot D:\Qt\5.14.1\msvc2017_64 `
  -VtkRoot D:\deps\VTK-8.2

.\scripts\Build-CMake.ps1
```

The executable and source-controlled runtime data are written to:

```text
out\runtime\Debug\
```

No E: drive, `subst`, junction, `binForPublic`, or global `PATH` change is used.

The legacy `Resource2.qrc` references images below the excluded
`binForPublic/` directory. CMake uses `cmake/Resource2.cmake.qrc`, which reads
the same files from the source-controlled `images/` directory while preserving
their original Qt resource aliases. The legacy qrc is left unchanged.

## Low-level run

Qt and VTK are supplied through a process-local `PATH`:

```powershell
.\scripts\Run-CMake.ps1 `
  -QtRoot D:\Qt\5.14.1\msvc2017_64 `
  -VtkRoot D:\deps\VTK-8.2 `
  -AdditionalRuntimePath D:\deps\dev701-x64-debug\bin
```

`AdditionalRuntimePath` must contain verified x64 Debug builds of Qwt,
QwtPlot3D, QGLViewer, and QSsh. It must not point to the known 32-bit DLLs in
`src/qwt/lib`, `src/qwtplot3d/lib`, or `src/QGLViewer/lib`.

## First Windows validation

Run the steps separately and retain the complete output. Their meanings are:

1. `Check-Environment.ps1` failure: toolchain or dependency contract issue.
2. `Configure-CMake.ps1` failure: package discovery or CMake model issue.
3. `Build-CMake.ps1` compile failure: source inventory, include, MOC/UIC, or
   compiler compatibility issue.
4. Link failure: missing or incompatible import library/VTK component.
5. Run failure: missing or incompatible DLL/plugin/runtime data.

The legacy build should also be run before and after validation. A CMake change
is not accepted if it breaks the existing build path.

Clean-build validation removes only generated output, then rebuilds everything:

```powershell
Remove-Item .\out -Recurse -Force
.\scripts\build.ps1
.\scripts\run.ps1
```
