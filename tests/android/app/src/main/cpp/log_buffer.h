//
//  log_buffer.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 19.03.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <iosfwd>

namespace cc {

    class log_buffer : public std::streambuf {
    public:

        log_buffer();

    private:

        static const size_t c_buffer_size = 128;

    private:

        char m_buffer[c_buffer_size]{};

    private:

        int overflow(int c);

        int sync();

    };

}