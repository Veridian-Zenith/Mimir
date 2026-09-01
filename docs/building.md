# Building Mimir

## Prerequisites

### Required

- **clang 22+** (C++26 support required)
- **CMake 3.28+**
- **Ninja** (build generator)
- **libclamav** (ClamAV development libraries)

### Optional

- **GTK4 + libadwaita** (for mimir-gtk)
- **Qt6** (for mimir-qt)

## Installing Dependencies

### Fedora/RHEL

```sh
dnf install clang cmake ninja-build clamav-devel gtk4-devel \
  libadwaita-devel qt6-qtbase-devel qt6-qtbase-private-devel
```

### Ubuntu/Debian

```sh
apt install clang-22 cmake ninja-build libclamav-dev \
  libgtk-4-dev libadwaita-1-dev qt6-base-dev
```

### Arch

```sh
pacman -S clang cmake ninja clamav gtk4 libadwaita qt6-base
```

## Build Presets

### GTK Frontend

```sh
cmake --preset gtk
cmake --build --preset gtk
```

Output: `build/gtk/mimir-gtk`

### Qt Frontend

```sh
cmake --preset qt
cmake --build --preset qt
```

Output: `build/qt/mimir-qt`

### Host Tests (future)

```sh
cmake --preset host
cmake --build --preset host
ctest --preset host
```

## ClamAV Setup

For full filesystem scanning, configure clamd to run as root:

```sh
sudo systemctl edit clamav-daemon
```

Add:
```ini
[Service]
User=root
AmbientCapabilities=CAP_DAC_READ_SEARCH
```

Then restart:
```sh
sudo systemctl restart clamav-daemon
```

See [SETUP.md](../WuMing/SETUP.md) for original reference.

## Troubleshooting

### "C++26 support required"

Ensure clang 22+ is installed and selected:
```sh
clang --version  # should be 22.x or higher
```

### "libclamav not found"

Install clamav-devel or rebuild ClamAV with --enable-development.

### "libadwaita not found"

Install libadwaita-1-dev (not -devel on some distros).
