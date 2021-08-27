//
//  controller.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 25.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <mutex>
#include <utility>

#define MINIAUDIO_IMPLEMENTATION

#include <miniaudio.h>

#include "maw/decoder.h"

#include "maw/controller.h"

namespace {

    std::mutex s_playing_mutex;

}

oo::controller::controller(std::function<void(oo::controller::command, const std::string &)> fn)
        : m_command_fn(std::move(fn)) {

}

oo::controller::~controller() {
    ma_device_uninit(m_device.get());
}

namespace {

    void data_callback(ma_device *device, void *output, const void *input, ma_uint32 frame_count) {
        auto output_f32 = static_cast<float *>(output);
        const auto channels = device->playback.channels;
        float buf[channels * frame_count];

        auto controller = static_cast<oo::controller *>(device->pUserData);
        bool end = true;

        std::lock_guard<std::mutex> lock(s_playing_mutex);
        for (const auto &[path, decoder]: controller->m_playing) {
            const auto read = decoder->read(buf, frame_count);
            for (size_t i = 0; i < read * channels; ++i) {
                output_f32[i] += buf[i];
            }

            //

            if (read < frame_count) {
                controller->m_command_fn(oo::controller::command::stop, path);
                controller->m_command_fn(oo::controller::command::reset, path);
            } else {
                end = false;
            }
        }

        if (end) {
            controller->m_command_fn(oo::controller::command::stop, "");
        }
    }

}

bool oo::controller::load(const std::string &path) {
    const auto decoder = std::make_shared<oo::decoder>();

    if (decoder->init(path)) {
        if (!m_device) {
            m_device = std::make_unique<ma_device>();

            ma_device_config device_config;
            device_config = ma_device_config_init(ma_device_type_playback);
            device_config.playback.format = decoder->get_output_format();
            device_config.playback.channels = decoder->get_output_channels();
            device_config.sampleRate = decoder->get_output_sample_rate();
            device_config.dataCallback = data_callback;
            device_config.pUserData = this;

            if (ma_device_init(nullptr, &device_config, m_device.get()) != MA_SUCCESS) {
                return false;
            }
        }

        m_decoders.emplace(path, decoder);

        return true;
    } else {
        return false;
    }
}

bool oo::controller::play(const std::string &path) {
    if (!m_device || m_device->state == 0) {
        return false;
    }

    if (m_device->state == 1) {
        if (ma_device_start(m_device.get()) != MA_SUCCESS) {
            return false;
        }
    }

    std::lock_guard<std::mutex> lock(s_playing_mutex);

    const auto &decoder = m_decoders[path];
    m_playing.emplace(path, decoder);

    return true;
}

bool oo::controller::stop(const std::string &path) {
    if (path.empty()) {
        return ma_device_stop(m_device.get()) == MA_SUCCESS;
    } else {
        std::lock_guard<std::mutex> lock(s_playing_mutex);

        m_playing.erase(path);
        return true;
    }
}

bool oo::controller::reset(const std::string &path) {
    const auto &decoder = m_decoders[path];
    return decoder->seek(0);
}