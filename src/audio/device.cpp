//
//  device.cpp
//  audio_engine
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 28.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <utility>

#include "audio/device.h"

namespace {

    void data_callback(ma_device *device, void *output, const void *, ma_uint32 frame_count) {
        auto output_f32 = static_cast<float *>(output);
        const auto &callback = *static_cast<oo::audio::device::callback *>(device->pUserData);

        const auto total = callback(output_f32, frame_count, device->playback.channels);
        
        if (total < frame_count) {
            ma_silence_pcm_frames(
                    ma_offset_pcm_frames_ptr(output, total, device->playback.format, device->playback.channels),
                    (frame_count - total), device->playback.format, device->playback.channels);
        }
    }

}

oo::audio::device::device(oo::audio::device::callback callback) : m_callback(std::move(callback)) {

}

oo::audio::device::~device() {
    ma_device_uninit(&m_device);
    ma_context_uninit(&m_context);
}

bool oo::audio::device::init(ma_format format, ma_uint32 channels, ma_uint32 sample_rate) {
    ma_context_config context_config = ma_context_config_init();
    context_config.coreaudio.sessionCategory = ma_ios_session_category_ambient;
    context_config.coreaudio.sessionCategoryOptions = ma_ios_session_category_option_mix_with_others;
    ma_context_init(nullptr, 0, &context_config, &m_context);

    ma_device_config device_config;
    device_config = ma_device_config_init(ma_device_type_playback);
    device_config.playback.format = format;
    device_config.playback.channels = channels;
    device_config.sampleRate = sample_rate;
    device_config.pUserData = &m_callback;
    device_config.dataCallback = data_callback;

    if (ma_device_init(&m_context, &device_config, &m_device) == MA_SUCCESS) {
        ma_device_set_master_volume(&m_device, m_volume);

        return true;
    } else {
        return false;
    }
}

bool oo::audio::device::is_inited() const {
    return m_device.state != 0;
}

bool oo::audio::device::start() {
    return ma_device_start(&m_device) == MA_SUCCESS;
}

bool oo::audio::device::is_started() const {
    return m_device.state == 2;
}

bool oo::audio::device::stop() {
    return ma_device_stop(&m_device) == MA_SUCCESS;
}

bool oo::audio::device::is_stopped() const {
    return m_device.state == 1;
}

void oo::audio::device::set_volume(float value) {
    m_volume = value;

    if (is_inited()) {
        ma_device_set_master_volume(&m_device, m_volume);
    }
}