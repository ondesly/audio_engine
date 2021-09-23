//
//  miniaudio_async_test.cpp
//  audio_engine
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 03.09.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <iostream>

#include <miniaudio.h>

#include "miniaudio_async_test.h"

oo::audio::miniaudio_async_test::miniaudio_async_test() {
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

        char command;
        while (true) {
            if (m_queue.pop(command)) {
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
            } else {
                std::unique_lock<std::mutex> lock(m_mutex);
                if (m_is_done) {
                    break;
                } else {
                    using namespace std::chrono_literals;
                    m_condition.wait_for(lock, 100ms);
                }
            }
        }

        ma_device_uninit(&device);
    });
}

oo::audio::miniaudio_async_test::~miniaudio_async_test() {
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_is_done = true;
        m_condition.notify_all();
    }

    m_service_thread->join();
}

void oo::audio::miniaudio_async_test::start() {
    m_queue.push('a');
    m_condition.notify_one();
}

void oo::audio::miniaudio_async_test::stop() {
    m_queue.push('o');
    m_condition.notify_one();
}