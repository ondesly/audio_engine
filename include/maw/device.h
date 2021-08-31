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

    using callback = std::function<void(float *output, ma_uint32 frame_count, ma_uint32 channel_count)>;

    class device {
    public:

        explicit device(maw::callback callback);

        ~device();

    public:

        bool init(ma_format format, ma_uint32 channels, ma_uint32 sample_rate);

        [[nodiscard]] bool is_inited() const;

        bool start();

        [[nodiscard]] bool is_started() const;

        bool stop();

        [[nodiscard]] bool is_stopped() const;

    private:

        maw::callback m_callback;
        ma_device m_device = {};

    };

}