# Sovereign Media Player — Open-Core Edition

<p align="center">
  <strong>The world's fastest cross-platform media engine — community UI, sovereign core.</strong>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Platform-macOS%20%7C%20Windows%20%7C%20Linux-blue?style=for-the-badge" />
  <img src="https://img.shields.io/badge/License-MIT%20(Frontend)-green?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Engine-Proprietary%20Freeware-orange?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Status-Open--Core-brightgreen?style=for-the-badge" />
</p>

---

## Features

| Feature | Status |
|---|---|
| 4K / 8K HEVC Playback at 60 FPS | Zero Dropped Frames |
| Apple Metal Direct GPU Rendering | 0.7% CPU Overhead |
| Live HLS / DASH / RTSP Streaming | 120ms Pre-Roll Buffer |
| Cross-Platform (macOS, Windows, Linux) | Universal Binary |
| Real-Time Telemetry HUD | FPS / CPU / RAM / Codec |
| Multi-Format Audio (AAC, MP3, FLAC) | Full Surround |
| Full Keyboard Shortcut Suite | Power-User Ready |
| Zero Data Collection | 100% Offline |

---

## Repository Architecture (Open-Core Model)

```text
Sovereign-Media-Player/
├── core_engine/                        # Pre-compiled closed-source engine
│   ├── include/sovereign_engine.h      # Public C-API (open for community)
│   ├── lib/
│   │   ├── libsovereign.a              # macOS Universal (arm64 + x86_64)
│   │   ├── libsovereign_win.lib        # Windows x64
│   │   └── libsovereign_linux.so       # Linux x86_64 / ARM64
│   └── LICENSE_PROPRIETARY_CORE.txt   # Freeware / No-Decompile license
├── frontend_ui/                        # 100% Open-Source Swift UI
│   ├── OpenSovereignPlayerUI.swift     # Main macOS GUI (Glassmorphic)
│   └── LICENSE_OPEN_SOURCE.txt        # MIT License
├── docs/
│   ├── API_REFERENCE.md               # C-API documentation
│   ├── BUILD.md                        # Build instructions
│   └── CONTRIBUTING.md                # Community guide
├── .github/
│   └── workflows/
│       └── build.yml                   # GitHub Actions CI/CD
├── build_open_core.py                  # Unified build script
└── README.md
```

> **How it works:** The `core_engine/` ships as **pre-compiled binaries only** — the Zero-Copy ring buffer, GPU shaders, and hardware decoder are protected intellectual property. The `frontend_ui/` is completely open-source (MIT). Community developers build new UIs, integrations, and ports by linking against the provided header and pre-compiled libraries.

---

## Quick Start

### Download & Run (Users)
```bash
# No build required!
# 1. Go to: https://github.com/sovereign-player/sovereign-media-player/releases
# 2. Download the .zip (macOS)
# 3. Unzip, drag SovereignPlayer.app to /Applications, and launch!
```

### Build from Source (Developers)
```bash
git clone https://github.com/sovereign-player/sovereign-media-player.git
cd sovereign-media-player
python3 build_open_core.py
# Output: dist/SovereignPlayer.app
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

> **Note:** Contributions to `core_engine/` are not accepted — it is closed-source.

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
