# Mimir

A modern, lightweight GUI frontend for [ClamAV](https://www.clamav.net/), written in C++26.

**Dual UI**: Choose GTK4 or Qt6. Same core, same features.

## Features

| Feature | Description |
|---------|-------------|
| Fast Scanning | Uses `clamdscan` (daemon-based) with `clamscan` fallback |
| Modern UI | GTK4/Libadwaita or Qt6 - your choice |
| AMOLED Theme | Vibrant BlackxRed optimized for OLED displays |
| Manual Control | No auto-quarantining - you decide what to do |
| Signature Management | Update ClamAV virus definitions from the app |
| Drag & Drop | Drop files/folders to scan instantly |

## Architecture

```
┌─────────────────┐     ┌──────────────────┐
│   mimir-gtk     │     │    mimir-qt      │
│   (GTK4 UI)     │     │    (Qt6 UI)       │
└────────┬────────┘     └────────┬─────────┘
         │                       │
         └───────────┬───────────┘
                     ▼
           ┌─────────────────┐
           │    libmimir      │
           │  (C++26 core)    │
           │  - ClamAV wrapper│
           │  - Scan engine   │
           │  - Theme system  │
           └─────────────────┘
                     │
                     ▼
              ┌───────────┐
              │  ClamAV   │
              └───────────┘
```

## Quick Start

```sh
# GTK4 build
cmake --preset gtk
cmake --build --preset gtk

# Qt6 build
cmake --preset qt
cmake --build --preset qt
```

See [docs/building.md](docs/building.md) for full setup.

## Requirements

- clang 22+ (C++26)
- CMake 3.28+
- GTK4 + libadwaita (for GTK UI)
- Qt6 (for Qt UI)
- ClamAV libraries (`libclamav`)

## License

OSL-3.0 - see LICENSE
