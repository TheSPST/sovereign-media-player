/**
 * ==============================================================================
 * SOVEREIGN MEDIA PLAYER — CROSS-PLATFORM OPEN-CORE DESKTOP PLAYER
 * (C) 2026 Sovereign Byte Technology. All Rights Reserved.
 *
 * Supported Platforms: Windows (Win32/D3D11/12), Linux (X11/Wayland/OpenGL), macOS
 * Zero-Bloat, Instant-On, Hardware-Accelerated High-Fidelity Playback
 * ==============================================================================
 */

#include "core_engine/include/sovereign_engine.h"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <atomic>
#include <csignal>

#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <shellapi.h>
#else
    #include <unistd.h>
    #include <termios.h>
#endif

namespace {

std::atomic<bool> g_running{true};

void handle_signal(int) {
    g_running = false;
}

std::string format_time(double seconds) {
    if (seconds < 0.0) seconds = 0.0;
    int total_sec = static_cast<int>(seconds);
    int hrs = total_sec / 3600;
    int mins = (total_sec % 3600) / 60;
    int secs = total_sec % 60;
    int frames = static_cast<int>((seconds - total_sec) * 30.0);

    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(2) << hrs << ":"
        << std::setfill('0') << std::setw(2) << mins << ":"
        << std::setfill('0') << std::setw(2) << secs << ":"
        << std::setfill('0') << std::setw(2) << frames;
    return oss.str();
}

void print_banner() {
    std::cout << "====================================================================\n";
    std::cout << "▶ SOVEREIGN MEDIA PLAYER (OPEN-CORE CROSS-PLATFORM EDITION)\n";
    std::cout << "  (C) 2026 Sovereign Byte Technology. All Rights Reserved.\n";
    std::cout << "  Engine Pipeline: Direct Zero-Copy GPU Acceleration\n";
    std::cout << "====================================================================\n";
    std::cout << "Controls:\n";
    std::cout << "  [Space] Play/Pause   | [← / →] Seek ±5s     | [↑ / ↓] Volume ±10%\n";
    std::cout << "  [S]     Stop         | [F]     Fullscreen   | [Q / Esc] Quit\n";
    std::cout << "====================================================================\n" << std::endl;
}

} // anonymous namespace

int main(int argc, char* argv[]) {
    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);

    print_banner();

    std::string media_path;
    if (argc > 1) {
        media_path = argv[1];
    } else {
        std::cout << "Enter video path or stream URL (HLS / DASH / MP4 / MKV): ";
        std::getline(std::cin, media_path);
    }

    if (media_path.empty()) {
        media_path = "sample_stream.mp4";
    }

    // 1. Initialize Hardware Engine
    if (!Sovereign_InitializeEngine()) {
        std::cerr << "❌ Failed to initialize Sovereign Hardware Engine context.\n";
        return 1;
    }

    // 2. Create Playback Session
    std::cout << "\n⏳ Opening media: " << media_path << " ...\n";
    SovereignSessionHandle session = Sovereign_CreateSession(media_path.c_str());
    if (!session) {
        std::cerr << "❌ Could not open media file or stream.\n";
        Sovereign_ShutdownEngine();
        return 1;
    }

    // 3. Start Playback
    Sovereign_Play(session);
    std::cout << "✔ Playback started with hardware-accelerated decoding.\n\n";

    double current_vol = 1.0;

    // Headless / Terminal Telemetry HUD Loop
    int frame_tick = 0;
    while (g_running) {
        SovereignTelemetry t = Sovereign_GetTelemetry(session);

        std::string cur_str = format_time(t.current_time);
        std::string dur_str = format_time(t.duration);

        // Render live HUD in-place
        std::cout << "\r[▶ PLAYING] " << cur_str << " / " << dur_str
                  << " | FPS: " << std::fixed << std::setprecision(1) << t.current_fps
                  << " | CPU: " << std::fixed << std::setprecision(1) << t.cpu_usage_percent << "%"
                  << " | RAM: " << std::fixed << std::setprecision(1) << t.ram_usage_mb << " MB"
                  << " | Drops: " << t.dropped_frames
                  << " | Vol: " << static_cast<int>(current_vol * 100) << "%   "
                  << std::flush;

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        frame_tick++;

        // Auto-loop for demo stream if duration reached
        if (t.duration > 0.0 && t.current_time >= t.duration) {
            std::cout << "\n\n✔ Reached end of stream. Restarting playback...\n";
            Sovereign_Seek(session, 0.0);
            Sovereign_Play(session);
        }

        // Optional quick termination for CLI automation
        if (argc > 2 && std::strcmp(argv[2], "--test-exit") == 0 && frame_tick >= 8) {
            break;
        }
    }

    std::cout << "\n\n🛑 Stopping playback and releasing hardware surfaces...\n";
    Sovereign_Pause(session);
    Sovereign_DestroySession(session);
    Sovereign_ShutdownEngine();

    std::cout << "✔ Shutdown complete.\n";
    return 0;
}
