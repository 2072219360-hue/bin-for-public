# Dependency baseline

This document records the conservative build contract. It describes the
existing known-good environment; it does not upgrade or replace dependencies.

| Component | Baseline | Required compatibility |
| --- | --- | --- |
| Operating system | Windows 10 or later | x64 |
| Visual Studio | Visual Studio 2017 | MSVC v141 x64, `/MDd` |
| CMake | 3.21 or later | Visual Studio 15 2017 generator |
| Qt | 5.14.1 `win64_msvc2017_64` | Debug libraries, Qt Charts installed |
| VTK | 8.2.0 | x64 Debug, shared, Qt 5 and OpenGL2 enabled |
| Qwt | 6.0.1 source snapshot | existing Debug x64 import library initially used |
| QGLViewer | 2.3.17 source snapshot | existing Debug x64 import library initially used |
| QwtPlot3D | vendored source snapshot, upstream revision unknown | existing Debug x64 import library initially used |
| QSsh | vendored source snapshot, upstream revision unknown | existing Debug x64 import library initially used |
| JsonCpp | vendored headers, upstream revision unknown | existing Debug x64 libraries initially used |
| Log4Qt | vendored source snapshot, upstream revision unknown | compiled as application sources |

## Known limitations

- Only `Windows + VS2017 + x64 + Debug` is supported by the initial CMake
  prototype. The legacy project files still contain Win32 and Release entries,
  but those configurations do not have a verified consistent dependency set.
- The committed Qwt, QwtPlot3D, and QGLViewer DLLs under their source trees are
  32-bit binaries. The CMake prototype deliberately does not deploy them.
- The committed import libraries must still be verified on the Windows build
  machine with `dumpbin`. They are temporary compatibility inputs, not the
  desired long-term dependency model.
- `mpisupport`, `pyengine`, and some cluster runtime assets referenced by the
  application are not present in this source repository.
- The source tree mixes legacy Chinese encodings and UTF-8. This migration does
  not convert source files or change the compiler's existing encoding behavior.

## Dependency policy for the next milestone

Each dependency must eventually record its upstream URL, exact tag or commit,
license, SHA256 (for archives), architecture, compiler toolset, runtime library,
Qt version, build type, and build options. Debug and Release artifacts must not
be inferred from filename aliases.
