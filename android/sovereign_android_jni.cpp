/**
 * ==============================================================================
 * SOVEREIGN MEDIA PLAYER — ANDROID NDK JNI BRIDGE
 * (C) 2026 Sovereign Byte Technology. All Rights Reserved.
 *
 * Direct Hardware Binding to Android MediaCodec, ANativeWindow & OpenSL ES
 * ==============================================================================
 */

#include <jni.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include <string>
#include "core_engine/include/sovereign_engine.h"

#define LOG_TAG "SovereignPlayerNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C" {

JNIEXPORT jboolean JNICALL
Java_tech_sovereign_player_SovereignPlayer_nativeInit(JNIEnv* env, jobject thiz) {
    LOGI("Initializing Sovereign Media Engine for Android NDK");
    return Sovereign_InitializeEngine() ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jlong JNICALL
Java_tech_sovereign_player_SovereignPlayer_nativeCreateSession(JNIEnv* env, jobject thiz, jstring j_url) {
    const char* url_str = env->GetStringUTFChars(j_url, nullptr);
    SovereignSessionHandle handle = Sovereign_CreateSession(url_str);
    env->ReleaseStringUTFChars(j_url, url_str);
    return reinterpret_cast<jlong>(handle);
}

JNIEXPORT jboolean JNICALL
Java_tech_sovereign_player_SovereignPlayer_nativeSetSurface(JNIEnv* env, jobject thiz, jlong session_handle, jobject surface) {
    SovereignSessionHandle handle = reinterpret_cast<SovereignSessionHandle>(session_handle);
    if (!handle || !surface) return JNI_FALSE;

    ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
    if (!window) {
        LOGE("Failed to obtain ANativeWindow from Surface");
        return JNI_FALSE;
    }

    bool attached = Sovereign_AttachSurface(handle, reinterpret_cast<void*>(window));
    ANativeWindow_release(window);
    return attached ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_tech_sovereign_player_SovereignPlayer_nativePlay(JNIEnv* env, jobject thiz, jlong session_handle) {
    SovereignSessionHandle handle = reinterpret_cast<SovereignSessionHandle>(session_handle);
    Sovereign_Play(handle);
}

JNIEXPORT void JNICALL
Java_tech_sovereign_player_SovereignPlayer_nativePause(JNIEnv* env, jobject thiz, jlong session_handle) {
    SovereignSessionHandle handle = reinterpret_cast<SovereignSessionHandle>(session_handle);
    Sovereign_Pause(handle);
}

JNIEXPORT void JNICALL
Java_tech_sovereign_player_SovereignPlayer_nativeSeek(JNIEnv* env, jobject thiz, jlong session_handle, jdouble time_seconds) {
    SovereignSessionHandle handle = reinterpret_cast<SovereignSessionHandle>(session_handle);
    Sovereign_Seek(handle, time_seconds);
}

JNIEXPORT void JNICALL
Java_tech_sovereign_player_SovereignPlayer_nativeDestroySession(JNIEnv* env, jobject thiz, jlong session_handle) {
    SovereignSessionHandle handle = reinterpret_cast<SovereignSessionHandle>(session_handle);
    Sovereign_DestroySession(handle);
}

JNIEXPORT void JNICALL
Java_tech_sovereign_player_SovereignPlayer_nativeShutdown(JNIEnv* env, jobject thiz) {
    Sovereign_ShutdownEngine();
}

} // extern "C"
