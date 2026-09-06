package tech.sovereign.player;

import android.view.Surface;

/**
 * Sovereign Media Player — Android Java/JNI High-Performance Wrapper
 * (C) 2026 Sovereign Byte Technology. All Rights Reserved.
 */
public class SovereignPlayer {
    static {
        System.loadLibrary("sovereign_android");
    }

    private long sessionHandle = 0;

    public boolean initialize() {
        return nativeInit();
    }

    public boolean openMedia(String url) {
        sessionHandle = nativeCreateSession(url);
        return sessionHandle != 0;
    }

    public boolean setSurface(Surface surface) {
        if (sessionHandle == 0) return false;
        return nativeSetSurface(sessionHandle, surface);
    }

    public void play() {
        if (sessionHandle != 0) nativePlay(sessionHandle);
    }

    public void pause() {
        if (sessionHandle != 0) nativePause(sessionHandle);
    }

    public void seek(double seconds) {
        if (sessionHandle != 0) nativeSeek(sessionHandle, seconds);
    }

    public void release() {
        if (sessionHandle != 0) {
            nativeDestroySession(sessionHandle);
            sessionHandle = 0;
        }
        nativeShutdown();
    }

    private native boolean nativeInit();
    private native long nativeCreateSession(String url);
    private native boolean nativeSetSurface(long handle, Surface surface);
    private native void nativePlay(long handle);
    private native void nativePause(long handle);
    private native void nativeSeek(long handle, double timeSec);
    private native void nativeDestroySession(long handle);
    private native void nativeShutdown();
}
