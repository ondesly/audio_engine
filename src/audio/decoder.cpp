//
//  decoder.cpp
//  audio_engine
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 26.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include "audio/decoder.h"

#include <utility>

oo::audio::decoder::decoder(std::string path) : m_path(std::move(path)) {

}

oo::audio::decoder::~decoder() {
    ma_decoder_uninit(&m_decoder);
}

std::string oo::audio::decoder::get_path() const {
    return m_path;
}

bool oo::audio::decoder::init() {
    return ma_decoder_init_file(m_path.c_str(), nullptr, &m_decoder) == MA_SUCCESS;
}

ma_uint64 oo::audio::decoder::read(float *output, float *buf, ma_uint64 frame_count, ma_uint64 channel_count) {
    const auto read = ma_decoder_read_pcm_frames(&m_decoder, buf, frame_count);
    for (size_t i = 0; i < read * channel_count; ++i) {
        output[i] += buf[i];
    }
    return read;
}

bool oo::audio::decoder::seek(ma_uint64 frame) {
    return ma_decoder_seek_to_pcm_frame(&m_decoder, frame) == MA_SUCCESS;
}

ma_format oo::audio::decoder::get_output_format() const {
    return m_decoder.outputFormat;
}

ma_uint32 oo::audio::decoder::get_output_channels() const {
    return m_decoder.outputChannels;
}

ma_uint32 oo::audio::decoder::get_output_sample_rate() const {
    return m_decoder.outputSampleRate;
}