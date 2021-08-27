//
//  decoder.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 26.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include "maw/decoder.h"

oo::decoder::~decoder() {
    ma_decoder_uninit(&m_decoder);
}

bool oo::decoder::init(const std::string &path) {
    return ma_decoder_init_file(path.c_str(), nullptr, &m_decoder) == MA_SUCCESS;
}

ma_uint64 oo::decoder::read(float *out, ma_uint64 count) {
    return ma_decoder_read_pcm_frames(&m_decoder, out, count);
}

bool oo::decoder::seek(ma_uint64 frame) {
    return ma_decoder_seek_to_pcm_frame(&m_decoder, frame) == MA_SUCCESS;
}

ma_format oo::decoder::get_output_format() const {
    return m_decoder.outputFormat;
}

ma_uint32 oo::decoder::get_output_channels() const {
    return m_decoder.outputChannels;
}

ma_uint32 oo::decoder::get_output_sample_rate() const {
    return m_decoder.outputSampleRate;
}