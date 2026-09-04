# Build Instructions

## Prerequisites

- **macOS:** Xcode Command Line Tools (`xcode-select --install`)
- **Windows:** Visual Studio 2022 with C++ Desktop Development workload
- **Linux:** `gcc`, `libx11-dev`, `libgl-dev`
- **Python 3.9+** for the build script

---

## macOS Build (Universal ARM64 + x86_64)

```bash
git clone https://github.com/sovereign-player/sovereign-media-player.git
cd sovereign-media-player
python3 build_open_core.py
```

**Output:**
- `dist/SovereignPlayer.app` — drag to `/Applications` or run directly
- `dist/SovereignPlayer_macOS.dmg` — installer image (requires `create-dmg`)

---

## Manual Swift Compile (macOS)

```bash
swiftc frontend_ui/OpenSovereignPlayerUI.swift \
  -L core_engine/lib -lsovereign \
  -framework Cocoa -framework AVKit -framework AVFoundation -framework Metal -framework MetalKit \
  -O -whole-module-optimization \
  -target arm64-apple-macosx11.0 \
  -Xlinker -dead_strip \
  -o dist/SovereignPlayer
```

---

## GitHub Actions (Automatic)

Every push to `main` triggers [`.github/workflows/build.yml`](.github/workflows/build.yml), which:
1. Compiles the Swift frontend for `arm64` and `x86_64`.
2. Creates a Universal Mach-O binary via `lipo`.
3. Packages `SovereignPlayer.app` into a `.zip`.
4. Uploads artifacts to the GitHub Release.

---

## Verifying the Build

```bash
# Check universal binary architecture
lipo -info dist/SovereignPlayer.app/Contents/MacOS/SovereignPlayer
# → Architectures in the fat file: arm64 x86_64

# Test run
open dist/SovereignPlayer.app
```
