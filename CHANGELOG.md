# Sovereign Media Player — Changelog

All notable changes to this project will be documented in this file.

---

## [v3.0.0] — 2026-09-04 (Open-Core GitHub Launch)

### Major: Open-Core GitHub Launch
- **Open-sourced** the full SwiftUI frontend under MIT License.
- **Decoupled** the proprietary engine into a clean public C-API (`sovereign_engine.h`).
- Published pre-compiled Universal binaries (arm64 + x86_64) for macOS.
- Added GitHub Actions CI/CD pipeline for automated builds and releases on every tag push.
- Added **media-playback-only** usage restriction to the engine license.

### Added
- `core_engine/include/sovereign_engine.h` — Public C-API with full doc comments.
- `docs/API_REFERENCE.md` — Full developer C-API documentation.
- `docs/BUILD.md` — Cross-platform build instructions.
- `docs/CONTRIBUTING.md` — Community contribution guide.
- `build_open_core.py` — Unified Python build pipeline.
- `.github/workflows/build.yml` — Automated GitHub Actions release workflow.

### Changed
- Removed hardware UUID licensing from the public frontend.
- Renamed `sovereign_player_gui_universal.swift` → `OpenSovereignPlayerUI.swift`.
- Removed all trademark symbols from UI strings.
- Cleaned all internal branding from public-facing files.

---

## [v2.0.0] — 2026-08-15

### Added
- Zero-Copy ring buffer (< 11 MB RAM).
- Apple Metal Direct GPU surface rendering.
- Live HLS / DASH / RTSP streaming support.
- Glassmorphic floating control bar.
- Real-time Telemetry HUD (FPS / CPU / RAM / Codec).

---

## [v1.0.0] — 2026-07-01

- Initial internal build.
- Basic AVKit hardware decoding pipeline.
- macOS Universal binary (arm64 + x86_64).
