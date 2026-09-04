# Sovereign Media Player — C-API Reference

## Overview

The `sovereign_engine.h` header exposes a clean, minimal C-API boundary between the open-source frontend and the closed-source proprietary engine. Link against `libsovereign.a` (macOS), `libsovereign_win.lib` (Windows), or `libsovereign_linux.so` (Linux) to use these functions.

---

## Session Lifecycle

### `Sovereign_InitializeEngine() → bool`
Boots the hardware decoder, Metal/DirectX/Vulkan context, and the Zero-Copy ring buffer pool. Call once at app startup.

```c
if (!Sovereign_InitializeEngine()) {
    fprintf(stderr, "Failed to initialize Sovereign Engine.\n");
    exit(1);
}
```

---

### `Sovereign_CreateSession(const char* media_url) → SovereignSessionHandle`
Creates a hardware-accelerated playback session for a local file path or network URL (HTTP/HTTPS/HLS/RTSP).

```c
SovereignSessionHandle s = Sovereign_CreateSession("/path/to/4k_video.mp4");
// or live stream:
SovereignSessionHandle s = Sovereign_CreateSession("https://example.com/live/stream.m3u8");
```

---

### `Sovereign_AttachSurface(session, native_view_ptr) → bool`
Binds the video output to a native OS window/view handle.
- **macOS:** Pass an `NSView*` cast to `void*`
- **Windows:** Pass an `HWND` cast to `void*`
- **Linux:** Pass an X11 `Window` or Wayland `wl_surface*` cast to `void*`

```c
Sovereign_AttachSurface(session, (__bridge void*)myNSView);
```

---

## Playback Controls

| Function | Description |
|---|---|
| `Sovereign_Play(session)` | Start / resume playback |
| `Sovereign_Pause(session)` | Pause playback |
| `Sovereign_Seek(session, time_seconds)` | Frame-accurate seek |
| `Sovereign_SetVolume(session, 0.0–1.0)` | Set volume level |

---

## Telemetry

### `Sovereign_GetTelemetry(session) → SovereignTelemetry`

Returns a snapshot of hardware performance metrics. Poll at ~60 Hz for a live HUD.

```c
typedef struct {
    double current_time;         // Playback position (seconds)
    double duration;             // Total duration (seconds, 0 if live)
    double cpu_usage_percent;    // Engine CPU overhead (typically < 1.0% on Metal)
    double ram_usage_mb;         // Ring buffer RAM usage (capped at 11 MB)
    double current_fps;          // Rendered frames per second
    int    dropped_frames;       // Cumulative dropped frames (always 0 on Metal)
    bool   is_live_stream;       // True if source is HLS/RTSP
} SovereignTelemetry;
```

---

## Cleanup

```c
Sovereign_DestroySession(session);  // Release decoder and surfaces
Sovereign_ShutdownEngine();          // Teardown Metal/DX12/Vulkan context
```

---

## Build Example (macOS)

```bash
swiftc frontend_ui/OpenSovereignPlayerUI.swift \
  -import-objc-header core_engine/include/sovereign_engine.h \
  -L core_engine/lib -lsovereign \
  -framework Cocoa -framework AVKit -framework Metal \
  -O -target arm64-apple-macosx11.0 \
  -o SovereignPlayer
```

---

## Build Example (Linux with GCC)

```bash
gcc my_frontend.c \
  -I core_engine/include \
  -L core_engine/lib -lsovereign_linux \
  -lGL -lX11 \
  -o sovereign_player
```
