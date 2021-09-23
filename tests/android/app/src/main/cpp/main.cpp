//
//  main.cpp
//  audio_engine
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 22.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <android/asset_manager_jni.h>
#include <iostream>
#include <jni.h>
#include <memory>
#include <string>

#include <audio/a_asset_manager_holder.h>
#include <audio/engine.h>

#include "a_audio_test.h"
#include "log_buffer.h"
#include "miniaudio_async_test.h"
#include "miniaudio_test.h"

namespace {

    std::string to_string(JNIEnv *env, jstring j_string) {
        jboolean is_copy;
        auto chars = env->GetStringUTFChars(j_string, &is_copy);

        std::string string(chars);

        if (is_copy == JNI_TRUE) {
            env->ReleaseStringUTFChars(j_string, chars);
        }

        return string;
    }

    std::unique_ptr<oo::audio::engine> s_engine;
    std::unique_ptr<oo::audio::a_audio_test> s_audio_test;
    std::unique_ptr<oo::audio::miniaudio_test> s_mini_test;
    std::unique_ptr<oo::audio::miniaudio_async_test> s_async_test;

}

extern "C" {

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_setAssetManager(JNIEnv *env,
                                                              jobject /* this */,
                                                              jobject j_asset_manager) {
    std::cout.rdbuf(new oo::log_buffer);

    oo::audio::a_asset_manager_holder holder{};
    holder.set(AAssetManager_fromJava(env, j_asset_manager));
}

// Audio engine

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_play(
        JNIEnv *env,
        jobject /* this */,
        jstring j_path) {
    if (!s_engine) {
        s_engine = std::make_unique<oo::audio::engine>();
    }

    const auto path = to_string(env, j_path);

    s_engine->play(path);
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_stop(
        JNIEnv *env,
        jobject /* this */,
        jstring j_path) {
    if (!s_engine) {
        s_engine = std::make_unique<oo::audio::engine>();
    }

    const auto path = to_string(env, j_path);

    s_engine->stop(path);
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_stopAll(
        JNIEnv *env,
        jobject /* this */) {
    s_engine->stop();
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_term(
        JNIEnv *env,
        jobject /* this */) {
    s_engine = nullptr;
}

// AAudio

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_aaOpen(
        JNIEnv *env,
        jobject /* this */) {
    s_audio_test = std::make_unique<oo::audio::a_audio_test>();
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_aaStart(
        JNIEnv *env,
        jobject /* this */) {
    s_audio_test->start();
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_aaStop(
        JNIEnv *env,
        jobject /* this */) {
    s_audio_test->stop();
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_aaClose(
        JNIEnv *env,
        jobject /* this */) {
    s_audio_test = nullptr;
}

// Miniaudio

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_miniInit(
        JNIEnv *env,
        jobject /* this */) {
    s_mini_test = std::make_unique<oo::audio::miniaudio_test>();
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_miniStart(
        JNIEnv *env,
        jobject /* this */) {
    s_mini_test->start();
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_miniStop(
        JNIEnv *env,
        jobject /* this */) {
    s_mini_test->stop();
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_miniUninit(
        JNIEnv *env,
        jobject /* this */) {
    s_mini_test = nullptr;
}

// Miniaudio Async

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_asyncInit(
        JNIEnv *env,
        jobject /* this */) {
    s_async_test = std::make_unique<oo::audio::miniaudio_async_test>();
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_asyncStart(
        JNIEnv *env,
        jobject /* this */) {
    s_async_test->start();
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_asyncStop(
        JNIEnv *env,
        jobject /* this */) {
    s_async_test->stop();
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_audio_engine_test_MainActivity_asyncUninit(
        JNIEnv *env,
        jobject /* this */) {
    s_async_test = nullptr;
}

}