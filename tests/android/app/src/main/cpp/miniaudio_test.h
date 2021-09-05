//
//  miniaudio_test.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 03.09.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <miniaudio.h>

namespace maw {

    class miniaudio_test {
    public:

        miniaudio_test();

        ~miniaudio_test();

    public:

        void start();

        void stop();

    private:

        ma_device m_device = {};

    };

}