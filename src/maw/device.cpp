//
//  device.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 28.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <utility>

#include "maw/device.h"

namespace {

    void data_callback(ma_device *device, void *output, const void *, ma_uint32 frame_count) {
        auto output_f32 = static_cast<float *>(output);
        const auto channel_count = device->playback.channels;
        const auto &callback = *static_cast<maw::callback *>(device->pUserData);

        callback(output_f32, frame_count, channel_count);
    }

}

maw::device::device(maw::callback callback) : m_callback(std::move(callback)) {

}

maw::device::~device() {
    ma_device_uninit(&m_device);
}

bool maw::device::init(ma_format format, ma_uint32 channels, ma_uint32 sample_rate) {
    ma_device_config device_config;
    device_config = ma_device_config_init(ma_device_type_playback);
    device_config.playback.format = format;
    device_config.playback.channels = channels;
    device_config.sampleRate = sample_rate;
    device_config.pUserData = &m_callback;
    device_config.dataCallback = data_callback;

    return ma_device_init(nullptr, &device_config, &m_device) == MA_SUCCESS;
}

bool maw::device::is_inited() const {
    return m_device.state != 0;
}

bool maw::device::start() {
    return ma_device_start(&m_device) == MA_SUCCESS;
}

bool maw::device::is_started() const {
    return m_device.state == 2;
}

bool maw::device::stop() {
    return ma_device_stop(&m_device) == MA_SUCCESS;
}

bool maw::device::is_stopped() const {
    return m_device.state == 1;
}