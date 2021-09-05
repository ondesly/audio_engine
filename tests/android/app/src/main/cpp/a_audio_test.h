//
//  a_audio_test.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 03.09.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <AAudio/AAudio.h>

namespace maw {

    class a_audio_test {
    public:

        a_audio_test();

        ~a_audio_test();

    public:

        void start();

        void stop();

    private:

        AAudioStream *m_stream = nullptr;

    };

}