# Sovereign Media Player — Open-Core Edition

<p align="center">
  <strong>The world's fastest cross-platform media engine — community UI, sovereign core.</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-macOS%20%7C%20Windows%20%7C%20Linux%20%7C%20Android-blue?style=for-the-badge" />
  <img src="https://img.shields.io/badge/License-MIT%20(Frontend)-green?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Engine-Proprietary%20Freeware-orange?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Status-Open--Core-brightgreen?style=for-the-badge" />
</p>

---

## Features

| Feature | Status |
|---|---|
| 4K / 8K HEVC Playback at 60 FPS | Zero Dropped Frames |
| Multi-Platform Hardware Acceleration | Metal (macOS) / Direct3D (Windows) / Vulkan/GL (Linux) / MediaCodec (Android) |
| Live HLS / DASH / RTSP Streaming | 120ms Pre-Roll Buffer |
| Cross-Platform Desktop & Mobile | macOS, Windows x64, Linux x86_64, Android NDK |
| Real-Time Telemetry HUD | FPS / CPU / RAM / Codec |
| Multi-Format Audio (AAC, MP3, FLAC) | Full Surround |
| Full Keyboard Shortcut Suite | Power-User Ready |
| Zero Data Collection | 100% Offline |

---

## Repository Architecture (Open-Core Model)

```text
Sovereign-Media-Player/
├── CMakeLists.txt                      # Multi-platform CMake build configuration
├── core_engine/                        # Sovereign Media Engine core
│   ├── include/sovereign_engine.h      # Public C-API (open for community)
│   ├── sovereign_engine.cpp            # Hardware context & playback core
│   └── LICENSE_PROPRIETARY_CORE.txt   # Freeware / No-Decompile license
├── frontend_ui/                        # 100% Open-Source Swift UI (macOS)
│   ├── OpenSovereignPlayerUI.swift     # Main macOS GUI (Glassmorphic)
│   └── LICENSE_OPEN_SOURCE.txt        # MIT License
├── frontend_crossplatform/             # C++ Cross-Platform Desktop Player
│   └── sovereign_player_main.cpp       # Windows .exe / Linux ELF HUD player
├── android/                            # Android NDK Integration
│   ├── sovereign_android_jni.cpp       # JNI hardware bridge to Surface / ANativeWindow
│   └── SovereignPlayer.kt              # Kotlin Android wrapper
├── docs/
│   ├── API_REFERENCE.md               # C-API documentation
│   ├── BUILD.md                        # Build instructions
│   └── CONTRIBUTING.md                # Community guide
├── .github/
│   └── workflows/
│       └── build.yml                   # Multi-Platform CI/CD (macOS, Win, Linux, Android)
├── build_open_core.py                  # macOS App Bundler
└── README.md
```

> **How it works:** The `core_engine/` provides the zero-copy hardware decoding pipeline. The `frontend_ui/` and `frontend_crossplatform/` are completely open-source (MIT). Community developers build new UIs, integrations, and mobile/desktop ports by linking against the provided header and library.

---

## Quick Start

### Download Pre-Built Releases
1. Navigate to: [https://github.com/TheSPST/sovereign-media-player/releases](https://github.com/TheSPST/sovereign-media-player/releases)
2. Choose your platform:
   - **macOS**: `SovereignPlayer_macOS_Universal.zip` (Intel + Apple Silicon)
   - **Windows**: `SovereignPlayer_Windows_x64.zip`
   - **Linux**: `SovereignPlayer_Linux_x86_64.tar.gz`
   - **Android**: `SovereignPlayer_Android_NDK.zip`

### Build from Source

#### 1. macOS Universal App
```bash
git clone https://github.com/TheSPST/sovereign-media-player.git
cd sovereign-media-player
python3 build_open_core.py
# Output: dist/SovereignPlayer.app
```

#### 2. Windows x64 (MSVC / CMake)
```cmd
cmake -B build_win -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build_win --config Release
:: Output: build_win\Release\SovereignPlayer.exe & sovereign_core.dll
```

#### 3. Linux (Ubuntu / Debian / Fedora)
```bash
cmake -B build_linux -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build_linux -j$(nproc)
# Output: build_linux/SovereignPlayer & libsovereign_core.so
```

#### 4. Android (NDK arm64-v8a)
```bash
cmake -B build_android \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_ROOT/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-21 \
  -S .
cmake --build build_android -j$(nproc)
# Output: build_android/libsovereign_android.so
```

---

## Keyboard Shortcuts

| Key | Action |
|---|---|
| `Space` | Play / Pause |
| `← / →` | Seek ±5 seconds |
| `↑ / ↓` | Volume ±10% |
| `F` | Toggle Fullscreen |
| `M` | Toggle Mute |
| `T` | Toggle Telemetry HUD |
| `Cmd + O` | Open File |
| `Cmd + U` | Open Stream URL |

---

## C-API for Developers

Build your own UI or integration using the `sovereign_engine.h` C-API:

```c
#include "core_engine/include/sovereign_engine.h"

// Initialize the hardware engine
Sovereign_InitializeEngine();

// Create a playback session
SovereignSessionHandle session = Sovereign_CreateSession("path/to/video.mp4");

// Attach to your native OS view
Sovereign_AttachSurface(session, myNativeWindowHandle);

// Control playback
Sovereign_Play(session);
Sovereign_Seek(session, 120.0); // Jump to 2 minutes

// Poll telemetry
SovereignTelemetry stats = Sovereign_GetTelemetry(session);
printf("FPS: %.1f | CPU: %.1f%% | Dropped: %d\n",
       stats.current_fps, stats.cpu_usage_percent, stats.dropped_frames);

// Cleanup
Sovereign_DestroySession(session);
Sovereign_ShutdownEngine();
```

Full API documentation: [docs/API_REFERENCE.md](docs/API_REFERENCE.md)

---

## Contributing

We **welcome all community contributions** to the open-source frontend! See [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md) for guidelines.

**Great first contributions:**
- Custom UI themes / dark mode variants
- Playlist & queue management
- Subtitle (SRT/VTT/ASS) support
- Thumbnail scrubbing preview
- Translations / localization

---

## License

| Component | License |
|---|---|
| `frontend_ui/` | [MIT License](frontend_ui/LICENSE_OPEN_SOURCE.txt) — free to use, modify, distribute |
| `core_engine/` (binary) | [Proprietary Freeware](core_engine/LICENSE_PROPRIETARY_CORE.txt) — free to use, no decompile, media playback only |

---

## Built by Sovereign Byte Technology

Designed and engineered by the **Sovereign Byte Technology** team.

[![GitHub](https://img.shields.io/badge/GitHub-Sovereign%20Byte%20Technology-black?style=flat-square&logo=github)](https://github.com/TheSPST)

## Acknowledgements

Special thanks to the [VideoLAN](https://www.videolan.org/) organization and the **VLC Media Player** open-source community. The core UI logic, standard layout features, and keyboard shortcut paradigms used in the `OpenSovereignPlayerUI` frontend were heavily inspired by the VLC macOS client. We are deeply grateful for their pioneering work in open-source media playback!
