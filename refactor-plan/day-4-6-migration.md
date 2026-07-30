# Day 4-6 Migration Delivery

## Migrated foundation module

The legacy `argData` value object has a replacement path under `src_next`:

```text
domain/GridConversionOptions
    -> infrastructure/KeyValueGridConversionOptionsStore
    -> app/GridConversionModule
```

The new type preserves the legacy data responsibility (source path, x/y/z
dimensions, interval) without retaining a raw-pointer `copy` API, friendship,
or global application dependency. The key-value store is deliberately simple
and versionable. It is not yet a reader for a legacy input file; that adapter
will be added only when a caller is migrated.

## Module registration

`ModuleRegistry` owns modules with `std::unique_ptr`, rejects duplicate ids,
orders startup by `ModuleDescriptor::startupOrder`, and shuts down started
modules in reverse order after a failed initialization. This gives the legacy
application a single future integration point rather than new direct feature
construction in `huangmainwindow`.

## UI/service boundary

`GridSimplificationPageController` is intentionally free of Qt and VTK. Its
only collaborator is `IGridSimplificationService`; input and output are domain
`ResultDocument` values. The future Qt/Ribbon adapter owns controls and calls
this controller. It must not open result files or call `SceneWidget` directly.

## Integration status

No legacy source has been edited. The new implementations are ready for the
build workstream to compile as a separate target. The next application phase
will create the actual Ribbon adapter and migrate one call path through the
composition root.
