# Dev701QtNSPlatformBase

This repository contains the source required to build the Windows desktop
application. Generated output, installers, sample data, VTK binaries, and the
legacy `binForPublic` runtime bundle are deliberately excluded.

## Supported environment

- Windows 10 or later, x64
- Visual Studio 2017 Community or Build Tools, including C++ desktop tools
- Windows 10 SDK (the project was verified with 10.0.17763.0)
- Qt 5.14.1, `msvc2017_64`, with the Qt Charts module
- CMake and Git
- VTK 8.2 built for x64, Visual Studio 2017, and the same Qt installation

The exact project dependencies and preparation steps are described in
[`docs/DEVELOPMENT.md`](docs/DEVELOPMENT.md).

An additive, Debug-x64-only CMake migration is documented in
[`docs/CMAKE_BUILD.md`](docs/CMAKE_BUILD.md). It intentionally leaves the
legacy Visual Studio and qmake build files unchanged. The frozen dependency
contract and known binary limitations are in
[`docs/DEPENDENCIES.md`](docs/DEPENDENCIES.md).

For the new path, run `scripts\setup.ps1` once, then use the zero-argument
`scripts\build.ps1` and `scripts\run.ps1` commands.

## Quick start

In PowerShell, configure the current-user execution policy once (no administrator rights required):

```powershell
Set-ExecutionPolicy -Scope CurrentUser -ExecutionPolicy RemoteSigned -Force
```

For a fresh machine, run setup once. It discovers or records the local Qt, VTK, and runtime dependency locations:

```powershell
cd D:\Project
.\scripts\setup.ps1
```

Then build and run with zero arguments:

```powershell
.\scripts\build.ps1
.\scripts\run.ps1
```

The new chain removes the need for E: drive mappings, junctions, manual PATH changes, and repeated dependency path arguments. Build intermediates are stored in `out\build\vs2017-x64-debug`; the deployed executable and runtime files are stored in `out\runtime\Debug`, with logs in `out\runtime\Debug\logs`.

The legacy Visual Studio/qmake path remains available through `Build-Debug.ps1` and `Run-Debug.ps1` when explicit Qt and VTK roots are supplied.

## Repository contents

- `*.cpp`, `*.h`, `src`, `ui`, `images`, `resources`: application source and UI.
- `Qssh-master`, `src/qwt`, `src/qwtplot3d`, `src/QGLViewer`: source and the
  small version-pinned debug libraries required by the legacy Visual Studio
  project.
- `QtMsBuild`: Qt/MSBuild integration consumed by the project file.
- `scripts`: dependency provisioning, build, and launch entry points.

Do not commit `build`, `binForPublic`, VTK build output, installers, or local
data files. The `.gitignore` enforces this policy.
