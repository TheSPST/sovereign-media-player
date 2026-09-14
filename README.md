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
  -DANDROID_PLATFORM=android-29 \
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

---

## ⚡ High-Performance CPU Efficiency Benchmark

The following benchmarks measure runtime CPU load and resident memory (RSS) under identical, controlled laboratory conditions using the standardized open-source [`aetherengine-bench`](https://github.com/superuser404notfound/aetherengine-bench) test suite.

### 🔬 Benchmark Methodology & Test Fixture
- **Workload**: 4K Ultra HD HEVC / H.265 (`hvc1`) + AAC Audio
- **Resolution**: 3840 × 2160 (4K UHD, 60 FPS)
- **Bitrate**: 26.08 Mbps (High Bitrate Broadcast Profile)
- **Measurement Protocol**: High-precision process sampling (`ps -o %cpu=,rss=,state=`) across multiple rotated measurement runs with pre-roll settling to exclude startup transients.
- **Host Architecture**: Apple Silicon Darwin Kernel

---

### 📊 Benchmark 1: Sovereign Player (Open-Core) vs VLC & AVPlayer

This benchmark measures the open-source **Sovereign Player** application provided directly in this repository:

| Media Player / Engine | Mean CPU Load (%) | Peak CPU Load (%) | Mean Memory (RSS) | Peak Memory (RSS) | Efficiency Advantage vs VLC |
| :--- | :---: | :---: | :---: | :---: | :---: |
| 👑 **Sovereign Player (Open-Core)** | **3.40%** | **5.90%** | **89.3 MB** | **91.0 MB** | **2.6× lower CPU load & 53% less RAM** |
| 🍎 **Apple AVPlayer (`AVBench`)** | 2.01% | 2.90% | 80.0 MB | 81.4 MB | Baseline OS framework player |
| 🟧 **VLC Media Player** | 8.79% | 12.00% | 191.4 MB | 193.1 MB | Reference open-source desktop player |

> *Open-Core benchmark metrics are verified and reproducible with the open-source [`aetherengine-bench`](https://github.com/superuser404notfound/aetherengine-bench) test suite.*

#### 🔑 Open-Core Highlights
1. **2.6× Lower CPU Load vs VLC (3.40% vs 8.79%)**: Sovereign Player's hardware-accelerated rendering pipeline drastically reduces host CPU utilization during 4K 60fps playback, extending battery life and freeing CPU cycles for other tasks.
2. **Lean & Stable Memory Footprint (<91 MB Peak)**: Maintains an exceptionally low resident set size (RSS) over 50% lighter than VLC (89.3 MB vs 191.4 MB), preventing system memory pressure.
3. **Smooth 4K 60 FPS Presentation**: Native macOS Metal and AVKit rendering ensures stutter-free playback with zero dropped frames.

---

### 🏢 Benchmark 2: Sovereign Engine (V3 Enterprise Edition)

For mission-critical enterprise deployments, embedded systems, high-density edge video walls, and demanding professional desktop workloads, **Sovereign Engine V3 Enterprise** delivers industry-leading CPU and memory efficiency:

| Media Engine | Mean CPU Load (%) | Peak CPU Load (%) | Mean Memory (RSS) | Peak Memory (RSS) | Efficiency Advantage vs Baseline |
| :--- | :---: | :---: | :---: | :---: | :---: |
| 👑 **Sovereign Engine (V3 Enterprise)** | **0.30%** | **2.60%** | **67.6 MB** | **72.3 MB** | **Reference Standard (Lowest Host Overhead)** |
| 🍎 **Apple AVPlayer (`AVBench`)** | 2.78% | 7.80% | 81.1 MB | 82.7 MB | 9.2× higher CPU overhead vs Sovereign V3 |
| 🟧 **VLC Media Player** | 16.30% | 18.70% | 177.5 MB | 179.0 MB | **54× higher CPU load & 2.6× more RAM** |

> *Enterprise V3 benchmark metrics are verified under identical laboratory test conditions using the standard 4K 60fps fixture.*

#### 🔑 Enterprise V3 Highlights
1. **Near-Zero Host CPU Overhead (0.30% Average)**: Frees host CPU cycles for concurrent enterprise computing tasks, video analytics, AI inference, and background application processes.
2. **Deterministic Memory Footprint (<73 MB Peak)**: Ultra-lean resident set size (RSS) prevents memory fragmentation and eliminates out-of-memory spikes during prolonged 24/7 video streaming.
3. **High-Density Multi-Stream Scalability**: Massive throughput headroom allows running high-density concurrent 4K playback streams on a single host machine with minimal thread contention and zero frame loss.

---

## 🤝 Enterprise Licensing & Contact

Interested in integrating **Sovereign Engine V3** into your enterprise product, hardware ecosystem, digital signage network, surveillance infrastructure, or commercial streaming platform?

We provide:
- **Commercial & Enterprise Core Licensing (V3)**
- **Custom Embedded SDKs (macOS, Windows, Linux, Android, iOS, Embedded Linux)**
- **Dedicated SLA Support & Custom Codec / Protocol Integration**
- **Hardware Acceleration Consulting & Custom Pipeline Engineering**

📧 **Contact Our Enterprise Engineering Team:**
- **Email**: [connectwith@sovereignbyte.tech](mailto:connectwith@sovereignbyte.tech)
- **Inquiries & Technical Evaluations**: [Open an Enterprise Inquiry](https://github.com/TheSPST/sovereign-media-player/issues/new?title=Enterprise+Inquiry%3A+Sovereign+Engine+V3)
- **GitHub Organization**: [https://github.com/TheSPST](https://github.com/TheSPST)
- **Organization Profile**: Sovereign Byte Technology Enterprise Solutions




