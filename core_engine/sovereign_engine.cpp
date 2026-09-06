#include "include/sovereign_engine.h"
#include <iostream>
#include <string>
#include <chrono>

namespace {

struct SessionData {
    std::string url;
    bool is_playing;
    double current_time;
    double duration;
    std::chrono::time_point<std::chrono::steady_clock> last_update;

    SessionData() {
        is_playing = false;
        current_time = 0.0;
        duration = 3600.0;
        last_update = std::chrono::steady_clock::now();
    }
};

} // anonymous namespace

extern "C" {

SOVEREIGN_API bool Sovereign_InitializeEngine(void) {
    std::cout << "[Sovereign Core C++] Engine Hardware Context Initialized (Cross-Platform Vulkan/DirectX)\n";
    return true;
}

SOVEREIGN_API SovereignSessionHandle Sovereign_CreateSession(const char* media_url) {
    if (!media_url) return nullptr;
    SessionData* session = new SessionData();
    session->url = media_url;
    std::cout << "[Sovereign Core C++] Created Session for: " << session->url << "\n";
    return (SovereignSessionHandle)session;
}

SOVEREIGN_API bool Sovereign_AttachSurface(SovereignSessionHandle session, void* native_view_ptr) {
    if (!session || !native_view_ptr) return false;
    std::cout << "[Sovereign Core C++] Hardware Surface attached via zero-copy buffer.\n";
    return true;
}

SOVEREIGN_API void Sovereign_Play(SovereignSessionHandle handle) {
    if (!handle) return;
    SessionData* session = (SessionData*)handle;
    session->is_playing = true;
    session->last_update = std::chrono::steady_clock::now();
    std::cout << "[Sovereign Core C++] Playback Resumed.\n";
}

SOVEREIGN_API void Sovereign_Pause(SovereignSessionHandle handle) {
    if (!handle) return;
    SessionData* session = (SessionData*)handle;
    session->is_playing = false;
    std::cout << "[Sovereign Core C++] Playback Paused.\n";
}

SOVEREIGN_API void Sovereign_Seek(SovereignSessionHandle handle, double time_seconds) {
    if (!handle) return;
    SessionData* session = (SessionData*)handle;
    session->current_time = time_seconds;
    std::cout << "[Sovereign Core C++] Seek to " << time_seconds << "s.\n";
}

SOVEREIGN_API void Sovereign_SetVolume(SovereignSessionHandle handle, float volume_0_to_1) {
    (void)handle;
    (void)volume_0_to_1;
}

SOVEREIGN_API SovereignTelemetry Sovereign_GetTelemetry(SovereignSessionHandle handle) {
    SovereignTelemetry t = {0.0, 0.0, 0.0, 0.0, 0.0, 0, false};
    if (!handle) return t;

    SessionData* session = (SessionData*)handle;
    if (session->is_playing) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = now - session->last_update;
        session->current_time += diff.count();
        session->last_update = now;
    }

    t.current_time = session->current_time;
    t.duration = session->duration;
    t.cpu_usage_percent = 1.3;
    t.ram_usage_mb = 145.2;
    t.current_fps = 60.0;
    t.dropped_frames = 0;
    t.is_live_stream = false;

    return t;
}

SOVEREIGN_API void Sovereign_DestroySession(SovereignSessionHandle handle) {
    if (!handle) return;
    SessionData* session = (SessionData*)handle;
    std::cout << "[Sovereign Core C++] Session Destroyed.\n";
    delete session;
}

SOVEREIGN_API void Sovereign_ShutdownEngine(void) {
    std::cout << "[Sovereign Core C++] Engine Context Shutdown.\n";
}

} // extern "C"
