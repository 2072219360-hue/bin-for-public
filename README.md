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

## Quick start

From PowerShell in a fresh clone:

```powershell
Set-ExecutionPolicy -Scope Process Bypass
.\scripts\Install-Qt-5.14.1.ps1 -QtRoot D:\\Qt\\5.14.1\\msvc2017_64
.\scripts\Build-Vtk-8.2.ps1 -QtRoot D:\\Qt\\5.14.1\\msvc2017_64 -VtkRoot D:\\deps\\VTK-8.2
.\scripts\Build-Debug.ps1 -QtRoot D:\\Qt\\5.14.1\\msvc2017_64 -VtkRoot D:\\deps\\VTK-8.2
.\scripts\Run-Debug.ps1 -QtRoot D:\\Qt\\5.14.1\\msvc2017_64 -VtkRoot D:\\deps\\VTK-8.2
```

The first VTK build is expected to take substantial time. Subsequent project
builds use `build\\Debug-x64` and can be repeated with `Build-Debug.ps1`.

## Repository contents

- `*.cpp`, `*.h`, `src`, `ui`, `images`, `resources`: application source and UI.
- `Qssh-master`, `src/qwt`, `src/qwtplot3d`, `src/QGLViewer`: source and the
  small version-pinned debug libraries required by the legacy Visual Studio
  project.
- `QtMsBuild`: Qt/MSBuild integration consumed by the project file.
- `scripts`: dependency provisioning, build, and launch entry points.

Do not commit `build`, `binForPublic`, VTK build output, installers, or local
data files. The `.gitignore` enforces this policy.
