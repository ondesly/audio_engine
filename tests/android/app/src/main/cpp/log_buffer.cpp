//
//  log_buffer.cpp
//  audio_engine
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 19.03.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <streambuf>

#include <android/log.h>

#include "log_buffer.h"

namespace {

    const char *c_tag = "maw";

}

oo::log_buffer::log_buffer() {
    setp(m_buffer, m_buffer + c_buffer_size - 1);
}

int oo::log_buffer::overflow(int c) {
    if (c == traits_type::eof()) {
        *pptr() = traits_type::to_char_type(c);
        sbumpc();
    }
    return this->sync() ? traits_type::eof() : traits_type::not_eof(c);
}

int oo::log_buffer::sync() {
    __android_log_print(ANDROID_LOG_INFO, c_tag, "");

    if (pbase() != pptr()) {
        __android_log_print(ANDROID_LOG_INFO, c_tag, "%s",
                            std::string(pbase(), pptr() - pbase()).c_str());
        setp(m_buffer, m_buffer + c_buffer_size - 1);
    }
    return 0;
}