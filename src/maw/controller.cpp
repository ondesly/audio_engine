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

#include "maw/controller.h"

namespace {

    std::mutex s_playing_mutex;

}

oo::controller::controller(std::function<void(command, const std::string &)> fn) : m_command_fn (std::move(fn)) {

}

oo::controller::~controller() {
    for (const auto &[path, decoder]: m_decoders) {
        ma_decoder_uninit(decoder);
        delete decoder;
    }

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
            const auto read = ma_decoder_read_pcm_frames(decoder, buf, frame_count);
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
    auto decoder = new ma_decoder;

    if (ma_decoder_init_file(path.c_str(), nullptr, decoder) == MA_SUCCESS) {
        if (!m_device) {
            m_device = std::make_unique<ma_device>();

            ma_device_config device_config;
            device_config = ma_device_config_init(ma_device_type_playback);
            device_config.playback.format = decoder->outputFormat;
            device_config.playback.channels = decoder->outputChannels;
            device_config.sampleRate = decoder->outputSampleRate;
            device_config.dataCallback = data_callback;
            device_config.pUserData = this;

            if (ma_device_init(nullptr, &device_config, m_device.get()) != MA_SUCCESS) {
                delete decoder;
                return false;
            }
        }

        m_decoders.emplace(path, decoder);

        return true;
    } else {
        delete decoder;
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

    auto decoder = m_decoders[path];
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
    auto decoder = m_decoders[path];
    return ma_decoder_seek_to_pcm_frame(decoder, 0) == MA_SUCCESS;
}