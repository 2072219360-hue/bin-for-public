# Legacy Module Map

## Entry and application shell

| Area | Legacy location | Role | Migration status |
| --- | --- | --- | --- |
| Main window | `src/huangmainwindow.*` | Ribbon shell, menus, action wiring, central views | retain as adapter first |
| Signals and global state | `SignalInstance.*`, `ECHVSignalInstance.*` | cross-window communication | isolate behind app services |
| Workspaces and projects | `WorkSpaceOp*`, `projectWidget*`, `workSpaceStarter*` | workspace lifecycle | later migration |

## Domain and processing

| Area | Legacy location | Role | Migration status |
| --- | --- | --- | --- |
| Grid conversion options | `argData.*` | conversion path, dimensions, interval | first low-coupling candidate |
| Runtime grid state | `runData.*` | VTK grids and topology arrays | defer; split VTK data from domain state |
| Grid preprocessing | `src/gridpreprocess/` | grid transformations | migrate after contracts exist |
| Calculation | `src/calculate/`, `SSPNS*`, strategies | solver and workflow logic | high-risk, defer |
| Input parameters | `InputSet*`, `ParamSetDialog*`, `inputFileEditor*` | input model and editing | migrate after configuration port |

## Post-processing and visualization

| Area | Legacy location | Role | Migration status |
| --- | --- | --- | --- |
| Result browser and grid operations | `src/scenewidget.*` | PLT/VTK read, clipping, decimation, export, VTK view | high-risk core; extract by service |
| Lightweight result view | `src/scenewidget1.*` | VTK browser, field selection, display controls | reference for page adapter |
| Profiling window | `PostProcessProfiling.*` | PLT/VTK reader and VTK tool panels | merge selectively into adapters |
| Grid simplify page | `postgridsimplify.*` | thin page shell around `SceneWidget` | first page candidate |
| Qwt 3D view | `src/postprocessQWT3D/` | optional 3D rendering tools | plugin candidate; defer |

## Infrastructure and bundled dependencies

| Area | Legacy location | Role | Migration status |
| --- | --- | --- | --- |
| XML/JSON configuration | `src/xmlconfig/`, `src/json/` | configuration parsing | extract behind store ports |
| SSH | `Qssh-master/` | remote integration | retain as external adapter |
| Plot/view libraries | `src/qwt/`, `src/qwtplot3d/`, `src/QGLViewer/` | bundled third-party code | do not migrate; consume as dependencies |
| VTK | `VTK/`, root VTK DLLs | visualization runtime | do not expose from domain |

## Observed dependency risks

- The main window owns feature construction and action wiring, making pages
  difficult to test independently.
- `SceneWidget` combines file reading, VTK data ownership, processing,
  rendering, export, and UI controls in one large class.
- `runData` exposes VTK types in an otherwise application-level state object.
- Global signals and raw pointer ownership make lifecycle assumptions implicit.
