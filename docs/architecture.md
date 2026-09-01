# Mimir Architecture

## Overview

Mimir is a ClamAV GUI with separate GTK4 and Qt6 frontends sharing a common C++26 core.

## Components

### libmimir (Core Library)

The business logic layer with zero UI dependencies.

**Responsibilities:**
- ClamAV process spawning and output parsing
- Scan state management
- Signature/version info retrieval
- Settings persistence
- Theme token definitions

**Public API** (`include/mimir/`):
- `scanner.hh` - scan initiation and progress
- `signature.hh` - signature database info
- `settings.hh` - configuration
- `theme.hh` - color tokens and theme data

### mimir-gtk (GTK4 Frontend)

GTK4/Libadwaita application.

**Responsibilities:**
- GTK4 UI rendering
- Libadwaita widgets and patterns
- CSS-based theming from theme tokens
- Event handling -> core calls

### mimir-qt (Qt6 Frontend)

Qt6 application.

**Responsibilities:**
- Qt6 UI rendering
- QSS-based theming from theme tokens
- Signal/slot -> core calls

## Dependency Graph

```
libmimir
├── ClamAV (libclamav)
└── Standard library only

mimir-gtk
└── libmimir + GTK4 + libadwaita

mimir-qt
└── libmimir + Qt6
```

## Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| Separate frontends | GTK and Qt have incompatible object models |
| Core in C++26 | Modern language features, const correctness |
| Process-based ClamAV | libclamav API complexity vs CLI simplicity |
| Theme tokens in core | Single source of truth for both UIs |
| OSL-3.0 license | Permissive, matches other VZ projects |

## File Layout

```
libmimir/
├── include/mimir/
│   ├── scanner.hh
│   ├── signature.hh
│   ├── settings.hh
│   └── theme.hh
└── src/
    ├── scanner.cpp
    ├── signature.cpp
    ├── settings.cpp
    └── theme.cpp

mimir-gtk/
├── src/
│   ├── main.cpp
│   ├── application.cpp
│   ├── window.cpp
│   └── pages/
└── data/
    └── mimir-gtk.gresource.xml

mimir-qt/
├── src/
│   ├── main.cpp
│   ├── application.cpp
│   └── mainwindow.cpp
└── data/
    └── mimir-qt.qrc
```

## Decisions Log

| Date | Decision | Notes |
|------|----------|-------|
| 2026-09-01 | C++26 core library | Modern features, clang 22+ |
| 2026-09-01 | Separate GTK4/Qt6 frontends | No shared UI toolkit feasible |
| 2026-09-01 | Process-based ClamAV | CLI tools more maintainable than libclamav |
| 2026-09-01 | BlackxRed AMOLED theme | User preference for vibrant dark theme |
