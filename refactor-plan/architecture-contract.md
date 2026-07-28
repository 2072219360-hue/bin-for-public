# Application Architecture Contract

## Scope

This contract applies to all code under `src_next/`. Legacy code is an
integration boundary, not a dependency source for new business code.

## Layers

| Layer | Responsibility | Allowed dependencies |
| --- | --- | --- |
| `ui` | Qt/Ribbon views, presentation state, user input | `app`, domain contracts |
| `app` | startup, module registration, dependency composition | all `src_next` layers |
| `domain` | data models and business ports | C++ standard library |
| `algorithms` | calculation and transformation services | `domain` |
| `infrastructure` | files, databases, VTK/third-party adapters | `domain`, `algorithms` contracts |

## Rules

1. A UI page never reads or writes files and never calls VTK/Qwt directly.
2. Domain headers never include Qt, VTK, Qwt, QGLViewer, or QSsh headers.
3. Algorithms return domain data or explicit errors; they do not show dialogs.
4. Infrastructure implements domain ports and is supplied through `app`.
5. New code uses explicit ownership. Raw owning pointers and global mutable
   singletons are prohibited in `src_next`.
6. Each migrated legacy component gets an adapter at the boundary; its legacy
   type must not leak into public `src_next` interfaces.
