#ifndef SOVEREIGN_ENGINE_H
#define SOVEREIGN_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * ==============================================================================
 * Sovereign Media Engine — Public C-API (v3.0.0)
 * Copyright (c) 2026 Sovereign Byte Technology. All Rights Reserved.
 *
 * This header and its accompanying pre-compiled binary (libsovereign.a /
 * libsovereign.dylib / libsovereign.so) are governed by the Sovereign Media
 * Player Engine Proprietary Freeware License. See LICENSE_PROPRIETARY_CORE.txt.
 *
 * This engine is designed for decoding and rendering audio and video content
 * using platform hardware acceleration (Apple Metal, DirectX 12, Vulkan).
 * ==============================================================================
 */

/** Opaque handle to a Sovereign Video Session. */
typedef void* SovereignSessionHandle;

/**
 * Initialize the Sovereign Engine hardware context.
 * Call once at application startup before creating any sessions.
 * @return true on success, false if hardware initialization fails.
 */
bool Sovereign_InitializeEngine(void);

/**
 * Create a new media playback session.
 * @param media_url  Local file path or network URL (HLS, DASH, RTSP, HTTP/S).
 * @return           An opaque session handle, or NULL on failure.
 */
SovereignSessionHandle Sovereign_CreateSession(const char* media_url);

/**
 * Attach the hardware rendering surface to a native OS window/view.
 * @param session          A valid session handle.
 * @param native_view_ptr  macOS: NSView*  |  Windows: HWND  |  Linux: X11 Window*
 * @return true on success.
 */
bool Sovereign_AttachSurface(SovereignSessionHandle session, void* native_view_ptr);

/** Start or resume media playback. */
void Sovereign_Play(SovereignSessionHandle session);

/** Pause media playback. */
void Sovereign_Pause(SovereignSessionHandle session);

/**
 * Seek to an absolute timestamp.
 * @param time_seconds  Target position in seconds from the start.
 */
void Sovereign_Seek(SovereignSessionHandle session, double time_seconds);

/**
 * Set the audio output volume.
 * @param volume_0_to_1  Volume level from 0.0 (mute) to 1.0 (full).
 */
void Sovereign_SetVolume(SovereignSessionHandle session, float volume_0_to_1);

/**
 * Hardware performance telemetry snapshot.
 * Poll at ~60 Hz to drive a real-time stats overlay.
 */
typedef struct {
    double current_time;         /**< Current playback position (seconds). */
    double duration;             /**< Total duration in seconds (0.0 if live stream). */
    double cpu_usage_percent;    /**< Engine CPU overhead (typically < 1% on Metal). */
    double ram_usage_mb;         /**< Ring buffer RAM usage in MB. */
    double current_fps;          /**< Rendered frames per second. */
    int    dropped_frames;       /**< Cumulative dropped frames. */
    bool   is_live_stream;       /**< True if the source is a live HLS/RTSP stream. */
} SovereignTelemetry;

/**
 * Poll the engine for real-time hardware performance metrics.
 * @param session  A valid, active session handle.
 * @return         A filled SovereignTelemetry struct.
 */
SovereignTelemetry Sovereign_GetTelemetry(SovereignSessionHandle session);

/**
 * Destroy a session and release all associated hardware resources.
 * Must be called before Sovereign_ShutdownEngine().
 */
void Sovereign_DestroySession(SovereignSessionHandle session);

/**
 * Shut down the hardware engine context.
 * Call once at application exit after destroying all sessions.
 */
void Sovereign_ShutdownEngine(void);

#ifdef __cplusplus
}
#endif

#endif /* SOVEREIGN_ENGINE_H */
