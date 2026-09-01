# Mimir - Project Handbook

AGENTS.md - Project contract for any agent or human.

## Status

- Date: 2026-09-01
- Milestone: M0 - Skeleton and documentation
- State: Initial structure created

## Owner Preferences (binding)

1. Language: C++26 only (clang 22+). No C-only files except asm.
2. Toolchain: clang/lld/lldb/llvm-* exclusively. GCC rejected.
3. No Rust in this project.
4. License: OSL-3.0.
5. UI: Separate GTK4 and Qt6 frontends sharing core library.
6. Theme: AMOLED BlackxRed (spec in docs/themes.md).
7. Dependencies: Minimal. ClamAV (libclamav), GTK4/libadwaita, Qt6 only.
8. Comments: Rare, short, ASCII only. No line-wrapping at fixed columns.
9. Decisions: Append to docs/architecture.md "Decisions log" - never rewrite.

## Project Structure

```
Mimir/
├── libmimir/          Core library (shared by both UIs)
│   ├── include/mimir/ Public headers
│   └── src/          Implementation
├── mimir-gtk/         GTK4/Libadwaita frontend
├── mimir-qt/          Qt6 frontend
├── cmake/             CMake toolchain files
├── docs/              Architecture, themes, decisions
└── .github/workflows/ CI
```

## Dependency Law

```
mimir-gtk  ──►  libmimir
mimir-qt   ──►  libmimir
libmimir   ──►  (ClamAV only - no UI toolkit dependency)
```

Core has zero UI dependencies. Frontends only link against core, never each other.

## Read Order (fresh session)

1. AGENTS.md (this)
2. README.md
3. docs/architecture.md
4. docs/themes.md
5. docs/building.md

## Exact Commands

Configure + build GTK frontend:
```sh
cmake --preset gtk
cmake --build --preset gtk
```

Configure + build Qt frontend:
```sh
cmake --preset qt
cmake --build --preset qt
```

## Verification Checklist (run after ANY change)

1. `cmake --preset gtk` - zero warnings with -Wall -Wextra
2. `cmake --preset qt` - zero warnings with -Wall -Wextra
3. Both binaries link against libmimir (verify with ldd/ldd)
4. clang-tidy passes on libmimir/
5. No new dependencies without updating docs/architecture.md
