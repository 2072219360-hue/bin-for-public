# Ribbon Page Template

Every new Ribbon page is composed from four responsibilities:

1. A `PageDescriptor` declares the page id, title, category, and order.
2. A Qt adapter owns the Ribbon controls and central widget only.
3. A presenter/controller maps user intent to a domain service call.
4. A domain or algorithm service returns data/results without Qt widget types.

The first page candidate is `GridSimplificationPage`. Its initial adapter may
reuse the existing `postgridsimplify` view, but the new page must call an
`IGridSimplificationService` rather than access `reveal::SceneWidget` directly.

Required states: inactive, loading, ready, running, failed. Buttons are
enabled from these states rather than from ad-hoc widget checks.
