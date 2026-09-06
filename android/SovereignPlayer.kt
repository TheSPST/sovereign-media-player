package tech.sovereign.player

import android.view.Surface

/**
 * Sovereign Media Player — Android Kotlin High-Performance Wrapper
 * (C) 2026 Sovereign Byte Technology. All Rights Reserved.
 */
class SovereignPlayer {
    companion object {
        init {
            System.loadLibrary("sovereign_android")
        }
    }

    private var sessionHandle: Long = 0

    fun initialize(): Boolean = nativeInit()

    fun openMedia(url: String): Boolean {
        sessionHandle = nativeCreateSession(url)
        return sessionHandle != 0L
    }

    fun setSurface(surface: Surface): Boolean {
        if (sessionHandle == 0L) return false
        return nativeSetSurface(sessionHandle, surface)
    }

    fun play() {
        if (sessionHandle != 0L) nativePlay(sessionHandle)
    }

    fun pause() {
        if (sessionHandle != 0L) nativePause(sessionHandle)
    }

    fun seek(seconds: Double) {
        if (sessionHandle != 0L) nativeSeek(sessionHandle, seconds)
    }

    fun release() {
        if (sessionHandle != 0L) {
            nativeDestroySession(sessionHandle)
            sessionHandle = 0L
        }
        nativeShutdown()
    }

    // Native JNI functions
    private external fun nativeInit(): Boolean
    private external fun nativeCreateSession(url: String): Long
    private external fun nativeSetSurface(handle: Long, surface: Surface): Boolean
    private external fun nativePlay(handle: Long)
    private external fun nativePause(handle: Long)
    private external fun nativeSeek(handle: Long, timeSec: Double)
    private external fun nativeDestroySession(handle: Long)
    private external fun nativeShutdown()
}
