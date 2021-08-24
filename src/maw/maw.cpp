//
//  maw.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 15.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#define MINIAUDIO_IMPLEMENTATION

#include <miniaudio.h>

#include "maw/a_asset_vfs.h"

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

#if defined(ANDROID)
    oo::a_asset_vfs a_asset{};
    auto vfs = a_asset.create_vfs();

    if (ma_decoder_init_vfs(vfs, path.c_str(), nullptr, &decoder) != MA_SUCCESS) {
        a_asset.delete_vfs(vfs);
        delete payload;
        return;
    }
#else
    if (ma_decoder_init_file(path.c_str(), nullptr, &decoder) != MA_SUCCESS) {
        delete payload;
        return;
    }
#endif

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
    const auto result = ma_device_start(device);

    switch (result) {
        case MA_SUCCESS:
        case MA_INVALID_OPERATION:
            break;
        default:
            destroy_device(device);
            m_devices.erase(path);
            return;
    }
}

void oo::maw::stop(const std::string &path) {
    auto it = m_devices.find(path);
    if (it == m_devices.end()) {
        return;
    }

    auto device = it->second;
    auto result = ma_device_stop(device);

    switch (result) {
        case MA_SUCCESS:
        case MA_INVALID_OPERATION:
            break;
        default:
            destroy_device(device);
            m_devices.erase(path);
            return;
    }

    auto payload = static_cast<callback_payload *>(device->pUserData);
    auto &decoder = payload->decoder;
    result = ma_decoder_seek_to_pcm_frame(&decoder, 0);

    switch (result) {
        case MA_SUCCESS:
        case MA_INVALID_OPERATION:
            break;
        default:
            destroy_device(device);
            m_devices.erase(path);
            return;
    }
}

oo::maw::~maw() {
    for (const auto &[path, device] : m_devices) {
        destroy_device(device);
    }
    m_queue.set_done();
    m_service_thread->join();
}

void oo::maw::destroy_device(ma_device *device) {
    auto payload = static_cast<callback_payload *>(device->pUserData);

    ma_device_uninit(device);
    delete device;

    auto &decoder = payload->decoder;
    ma_decoder_uninit(&decoder);
    delete payload;
}

void oo::maw::run_service_thread() {
    m_service_thread = std::make_unique<std::thread>([&]() {
        while (m_queue.wait()) {
            auto device = m_queue.pop();
            if (!device) {
                continue;
            }

            ma_device_stop(device);

            auto payload = static_cast<callback_payload *>(device->pUserData);
            auto &decoder = payload->decoder;
            ma_decoder_seek_to_pcm_frame(&decoder, 0);
        }
    });
}
