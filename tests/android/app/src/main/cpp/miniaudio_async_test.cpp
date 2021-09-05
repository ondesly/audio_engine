//
//  miniaudio_async_test.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 03.09.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <iostream>

#include <miniaudio.h>

#include "miniaudio_async_test.h"

maw::miniaudio_async_test::miniaudio_async_test() {
    m_service_thread = std::make_unique<std::thread>([&]() {
        ma_device_config device_config;
        device_config = ma_device_config_init(ma_device_type_playback);
        device_config.playback.format = ma_format_f32;
        device_config.playback.channels = 2;
        device_config.sampleRate = 44100;
        device_config.dataCallback = [](ma_device *device, void *output,
                                        const void *,
                                        ma_uint32 frame_count) {
            std::cout << "callback" << std::endl;
        };

        ma_device device;
        auto result = ma_device_init(nullptr, &device_config, &device);
        if (result != MA_SUCCESS) {
            return;
        }

        while (!m_queue.is_done()) {
            const auto command = m_queue.pop();

            switch (command) {
                case 'a':
                    result = ma_device_start(&device);
                    if (result != MA_SUCCESS) {
                        return;
                    }
                    break;
                case 'o':
                    result = ma_device_stop(&device);
                    if (result != MA_SUCCESS) {
                        return;
                    }
                    break;
                default:

                    break;
            }
        }

        ma_device_uninit(&device);
    });
}

maw::miniaudio_async_test::~miniaudio_async_test() {
    m_queue.set_done();
    m_service_thread->join();
}

void maw::miniaudio_async_test::start() {
    m_queue.push('a');
}

void maw::miniaudio_async_test::stop() {
    m_queue.push('o');
}