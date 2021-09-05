//
//  miniaudio_test.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 03.09.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <iostream>

#include "miniaudio_test.h"

maw::miniaudio_test::miniaudio_test() {
    ma_device_config device_config;
    device_config = ma_device_config_init(ma_device_type_playback);
    device_config.playback.format = ma_format_f32;
    device_config.playback.channels = 2;
    device_config.sampleRate = 44100;
    device_config.dataCallback = [](ma_device *device, void *output, const void *,
                                    ma_uint32 frame_count) {
        std::cout << "callback" << std::endl;
    };

    auto result = ma_device_init(nullptr, &device_config, &m_device);
    if (result != MA_SUCCESS) {
        return;
    }
}

maw::miniaudio_test::~miniaudio_test() {
    ma_device_uninit(&m_device);
}

void maw::miniaudio_test::start() {
    auto result = ma_device_start(&m_device);
    if (result != MA_SUCCESS) {
        return;
    }
}

void maw::miniaudio_test::stop() {
    auto result = ma_device_stop(&m_device);
    if (result != MA_SUCCESS) {
        return;
    }
}