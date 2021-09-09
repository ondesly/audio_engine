//
//  device.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 28.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <functional>

#include <miniaudio.h>

namespace maw {

    class device {
    public:

        using callback = std::function<ma_uint64(float *output, ma_uint32 frame_count, ma_uint32 channel_count)>;

    public:

        explicit device(maw::device::callback callback);

        ~device();

    public:

        bool init(ma_format format, ma_uint32 channels, ma_uint32 sample_rate);

        [[nodiscard]] bool is_inited() const;

        bool start();

        [[nodiscard]] bool is_started() const;

        bool stop();

        [[nodiscard]] bool is_stopped() const;

    private:

        maw::device::callback m_callback;

        ma_context m_context = {};
        ma_device m_device = {};

    };

}