//
//  decoder.h
//  audio_engine
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 26.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <memory>
#include <string>

#include <miniaudio.h>

namespace oo::audio {

    class decoder {
    public:

        explicit decoder(std::string path);

        ~decoder();

    public:

        [[nodiscard]] std::string get_path() const;

        bool init();

        ma_uint64 read(float *output, float *buf, ma_uint64 frame_count, ma_uint64 channel_count);

        bool seek(ma_uint64 frame);

        [[nodiscard]] ma_format get_output_format() const;

        [[nodiscard]] ma_uint32 get_output_channels() const;

        [[nodiscard]] ma_uint32 get_output_sample_rate() const;

    private:

        const std::string m_path;
        ma_decoder m_decoder = {};

    };

}