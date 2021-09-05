//
//  miniaudio_async_test.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 03.09.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <memory>
#include <thread>

#include <blocking_queue/blocking_queue.h>

namespace maw {

    class miniaudio_async_test {
    public:

        miniaudio_async_test();

        ~miniaudio_async_test();

    public:

        void start();

        void stop();

    private:

        std::unique_ptr<std::thread> m_service_thread;
        oo::blocking_queue<char> m_queue;

    };

}