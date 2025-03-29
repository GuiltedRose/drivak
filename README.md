# Drivak

**Drivak** is a fully offline, moddable, cross-platform toolkit for Dungeon Masters.  
It’s built from the ground up to give you complete control over your worlds, your stories, and your sessions — no subscriptions, no logins, no fluff.

---

## Features

- **Cross-Platform**: Runs on Windows & Linux with native UI (macOS support coming later)
- **Fully Offline**: No servers, no DRM — your campaign stays local
- **Custom In-House Database**: Fast, flexible, and designed for storing world data, NPCs, dungeons, and more
- **Python & JSON Modding**: Build your own generators, tools, and world logic
- **Modular UI**: Organize your campaign your way with tabs, themes, and screen layouts
- **One-Time Purchase**: No subscription fees, ever — just $15 for the full experience (Steam build)

---

## Core Modules

- **GameDB**: Custom relational database for storing and querying world state
- **ModManager**: Load mods using Python scripts and/or JSON templates
- **UI System (Qt)**: Tabbed, minimal, and fully extensible
- **Save System**: Auto-versioned, lightweight save files with future-proof format

---

## Status

**In Development**  
Drivak is currently in active development and will be available soon on:
- GitHub (source-only)
- Steam (standalone prebuilt version)

You’re welcome to follow development, contribute, or compile your own builds.

---

## Build Instructions

### Dependencies:
- Qt 6.x (Core, Widgets, Gui)
- Python 3.x (with headers)
- CMake 3.16+
- C++20-capable compiler (GCC 10+, Clang 11+, MSVC 19.28+)

### Build:
```bash
git clone https://github.com/yourname/drivak.git
cd drivak
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
./build/bin/drivak
