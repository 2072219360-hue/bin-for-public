# Conservative CMake build

The CMake build is a parallel prototype. It does not edit or replace the
existing `.sln`, `.vcxproj`, `.pro`, or legacy PowerShell scripts. All generated
files go below `out/`.

## Scope

The first milestone supports only:

- Windows x64
- Visual Studio 2017 / MSVC v141
- Debug
- Qt 5.14.1 `msvc2017_64`
- VTK 8.2.0 built against the same Qt installation

## Configure and build

From PowerShell at the repository root:

```powershell
Set-ExecutionPolicy -Scope Process Bypass

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

## Run

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
