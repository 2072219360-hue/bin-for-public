# Development environment

## Dependency contract

The checked-in Visual Studio project is a legacy VS2017/x64 Debug project. All
developers must use compatible binary interfaces:

| Dependency | Required version / build |
| --- | --- |
| Compiler | MSVC 2017 x64 |
| Qt | 5.14.1 `msvc2017_64`, including Qt Charts |
| VTK | 8.2, x64 MSVC 2017, built against the same Qt root |
| Windows SDK | 10.0.17763.0 or a compatible installed Windows 10 SDK |

The repository retains a few small debug import libraries for Qwt, QwtPlot3D,
QGLViewer, and QSsh. They are part of the existing link contract and are not
substitutes for Qt or VTK.

## Initial setup

1. Clone the repository to a short local path, for example `D:\\work\\Dev701`.
2. Install Visual Studio 2017 C++ desktop tools and CMake.
3. Run `scripts\\Install-Qt-5.14.1.ps1` or provide an existing compatible Qt
   root.
4. Run `scripts\\Build-Vtk-8.2.ps1`. This downloads VTK source from its public
   upstream repository and installs a local VTK tree. It does not write VTK
   binaries into this Git repository.
5. Run `scripts\\Build-Debug.ps1` and then `scripts\\Run-Debug.ps1`.

## Why the build script creates compatibility links

The original `.vcxproj` uses the following absolute legacy paths:

- `E:\\Qt\\5.14.1\\msvc2017_64`
- `E:\\Project20260602\\Dev701QtNSPlatformBase`

`Build-Debug.ps1` creates junctions only when the corresponding path is absent.
It refuses to replace an existing non-matching path. This keeps the checked-in
project buildable while development proceeds. Replacing absolute paths with
project-relative MSBuild properties is an appropriate follow-up refactor.

## Build output and runtime

The source build output is:

```text
build\\Debug-x64\\Dev701QtNSPlatformBase.exe
```

`Run-Debug.ps1` sets a process-local `PATH` that supplies Qt, VTK, Qwt,
QwtPlot3D, QGLViewer, and QSsh DLLs. It does not depend on `binForPublic`.

## Excluded data

Files such as `*.vtk`, `*.dat`, `*.grd`, `*.plt`, and `*.gg` are test or user
data and are not source dependencies. Store shareable test data in a separate
artifact location and document how to obtain it; do not add it to this source
repository.
