//
//  main.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 22.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <android/asset_manager_jni.h>
#include <jni.h>
#include <memory>
#include <string>

#include <maw/a_asset_manager_holder.h>
#include <maw/player.h>

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

    std::unique_ptr<maw::player> s_player;

}

extern "C" {

JNIEXPORT void JNICALL
Java_com_github_ondesly_maw_test_MainActivity_play(
        JNIEnv *env,
        jobject /* this */,
        jstring j_path) {
    if (!s_player) {
        s_player = std::make_unique<maw::player>();
    }

    const auto path = to_string(env, j_path);

    s_player->play(path);
}

JNIEXPORT void JNICALL
Java_com_github_ondesly_maw_test_MainActivity_setAssetManager(JNIEnv *env,
                                                              jobject /* this */,
                                                              jobject j_asset_manager) {
    maw::a_asset_manager_holder holder{};
    holder.set(AAssetManager_fromJava(env, j_asset_manager));
}

}