//
//  main.cpp
//  cross_platform_template
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 22.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <android/asset_manager_jni.h>
#include <jni.h>

#include "maw/a_asset_vfs.h"
#include "maw/audio.h"

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

}

extern "C" {

JNIEXPORT void JNICALL
Java_com_github_ondesly_maw_test_MainActivity_play(
        JNIEnv *env,
        jobject /* this */,
        jstring j_path) {
    const auto path = to_string(env, j_path);

    oo::audio audio{};
    audio.load(path);
    audio.play(path);
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_maw_test_MainActivity_setAssetManager(JNIEnv *env,
                                                              jobject /* this */,
                                                              jobject j_asset_manager) {
    oo::a_asset_vfs vfs{AAssetManager_fromJava(env, j_asset_manager)};
}

}