# Dependencies and Migration Priority

## Target dependency graph

```text
Qt/Ribbon view -> page presenter -> domain service port
                                      ^
                              algorithm implementation
                                      ^
                      VTK/file/configuration adapter
```

The legacy main window remains outside this graph and activates new modules
through a small adapter during the coexistence period.

## Priority

| Priority | Component | Reason | Next action |
| --- | --- | --- | --- |
| P0 | `argData` | self-contained value object with no Qt/VTK dependency | replacement module delivered; legacy adapter pending |
| P0 | `postgridsimplify` | small page shell; good test of UI-to-service boundary | controller delivered; Qt/Ribbon adapter pending |
| P1 | `SceneWidget` file/decimation/clip functions | necessary post-processing capability but heavily coupled | split readers, services, and VTK adapters |
| P1 | `PostProcessProfiling` controls | overlaps with SceneWidget functionality | retain only unique view tools |
| P2 | `runData` | contains VTK ownership and topology state | separate domain metadata from VTK representation |
| P2 | XML/JSON configuration | reused infrastructure | implement store adapters after port selection |
| P3 | solver/workflow/project modules | broad global and UI coupling | defer until a stable app composition root exists |

## First migration boundary

`GridConversionOptions` is the new domain representation of the values in
`argData`. It is intentionally free of legacy classes. A later adapter will
translate between the legacy object and this type until legacy callers are
removed.

`GridSimplificationPage` is the first page boundary. It will contain Ribbon
controls and presentation state only. File loading, VTK operations, clipping,
decimation, and export will be requested through service interfaces.

## Deferred work

No `SceneWidget`, VTK reader, or legacy main-window source is modified in this
phase. Their behavior is preserved while contracts and tests are established.
