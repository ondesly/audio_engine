//
//  device.h
//  audio_engine
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 28.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <functional>

#include <miniaudio.h>

namespace oo::audio {

    class device {
    public:

        using callback = std::function<ma_uint64(float *output, ma_uint32 frame_count, ma_uint32 channel_count)>;

    public:

        explicit device(device::callback callback);

        ~device();

    public:

        bool init(ma_format format, ma_uint32 channels, ma_uint32 sample_rate);

        [[nodiscard]] bool is_inited() const;

        bool start();

        [[nodiscard]] bool is_started() const;

        bool stop();

        [[nodiscard]] bool is_stopped() const;

        void set_volume(float value);

    private:

        device::callback m_callback;

        ma_context m_context = {};
        ma_device m_device = {};

        float m_volume = 1.F;

    };

}