//
//  maw.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 15.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#define MINIAUDIO_IMPLEMENTATION

#include <miniaudio.h>

#include "maw/maw.h"

namespace {

    struct callback_payload {
        oo::waiting_queue<ma_device *> &queue;
        ma_decoder decoder;
    };

}

oo::maw::maw() {
    run_service_thread();
}

namespace {

    void data_callback(ma_device *device, void *output, const void *input, ma_uint32 frame_count) {
        auto payload = static_cast<callback_payload *>(device->pUserData);

        if (!payload) {
            return;
        }

        auto &decoder = payload->decoder;
        const auto read = ma_decoder_read_pcm_frames(&decoder, output, frame_count);
        if (read < frame_count) {
            payload->queue.push(device);
        }
    }

}

void oo::maw::load(const std::string &path) {
    auto payload = new callback_payload{m_queue, ma_decoder{}};

    auto &decoder = payload->decoder;
    if (ma_decoder_init_file(path.c_str(), nullptr, &decoder) != MA_SUCCESS) {
        delete payload;
        return;
    }

    ma_device_config device_config;
    device_config = ma_device_config_init(ma_device_type_playback);
    device_config.playback.format = decoder.outputFormat;
    device_config.playback.channels = decoder.outputChannels;
    device_config.sampleRate = decoder.outputSampleRate;
    device_config.pUserData = payload;
    device_config.dataCallback = data_callback;

    //

    auto device = new ma_device;
    if (ma_device_init(nullptr, &device_config, device) != MA_SUCCESS) {
        ma_decoder_uninit(&decoder);
        delete payload;
        delete device;
        return;
    }

    m_devices[path] = device;
}

void oo::maw::play(const std::string &path) {
    auto it = m_devices.find(path);
    if (it == m_devices.end()) {
        return;
    }

    auto device = it->second;
    if (ma_device_start(device) != MA_SUCCESS) {
        auto payload = static_cast<callback_payload *>(device->pUserData);

        ma_device_uninit(device);
        delete device;

        auto &decoder = payload->decoder;
        ma_decoder_uninit(&decoder);
        delete payload;

        m_devices.erase(path);
    }
}

void oo::maw::stop(const std::string &path) {
    auto it = m_devices.find(path);
    if (it == m_devices.end()) {
        return;
    }

    auto device = it->second;
    if (ma_device_stop(device) != MA_SUCCESS) {
        auto payload = static_cast<callback_payload *>(device->pUserData);

        ma_device_uninit(device);
        delete device;

        auto &decoder = payload->decoder;
        ma_decoder_uninit(&decoder);
        delete payload;

        m_devices.erase(path);
    }
}

oo::maw::~maw() {
    for (const auto &[path, device] : m_devices) {
        m_queue.push(device);
    }
    m_queue.set_done();
    m_service_thread->join();
}

void oo::maw::run_service_thread() {
    m_service_thread = std::make_unique<std::thread>([&]() {
        while (m_queue.wait()) {
            auto device = m_queue.pop();
            if (!device) {
                continue;
            }

            auto decoder = static_cast<ma_decoder *>(device->pUserData);

            ma_device_stop(device);
            ma_device_uninit(device);
            ma_decoder_uninit(decoder);
        }
    });
}
