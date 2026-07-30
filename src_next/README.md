# src_next

`src_next` is the only location for new product code. The legacy root and
`src/` remain unchanged while features are migrated incrementally.

## Layout

- `app/`: application startup and module registration contracts.
- `ui/`: Ribbon pages, presenters, and Qt adapters.
- `domain/`: business data and service contracts. This layer must not include
  Qt, VTK, Qwt, database, or filesystem headers.
- `algorithms/`: computation contracts and implementations. Algorithms depend
  on domain types, never on widgets.
- `infrastructure/`: filesystem, persistence, and third-party adapters.

## Dependency direction

`ui -> app -> domain <- algorithms <- infrastructure`

`ui` may request a domain service through an application module. It must not
open files, call VTK algorithms, or access global legacy singletons directly.
`infrastructure` implements domain ports and is injected by `app`.

## Naming and include rules

- Namespace: `dev701::next` (written as nested namespaces for C++11 support).
- Public headers live under `include/dev701/next/<module>/`.
- Use `#include <dev701/next/...>` for public cross-module includes.
- A public header may include only the C++ standard library and public headers
  from lower-level modules.
- Qt/VTK/Qwt types are confined to implementation files or explicit adapters.
- New code does not include files from the legacy root or legacy `src/`.

## Current status

The first migrated module is the grid conversion configuration path:
`GridConversionOptions`, `KeyValueGridConversionOptionsStore`, and
`GridConversionModule`. It is independent of the legacy `argData` class; a
legacy adapter is deferred until the application integration phase.

`ModuleRegistry` owns new module instances and starts them in declared order.
The legacy main window will later need only one composition-root call to
register the new modules. The `GridSimplificationPageController` demonstrates
the UI boundary: it accepts a service contract and result data, not a widget,
file path, VTK object, or legacy singleton.

These sources are deliberately not added to legacy project files. The
build-system workstream will add them as a separate target before the legacy
executable is wired to the composition root.
