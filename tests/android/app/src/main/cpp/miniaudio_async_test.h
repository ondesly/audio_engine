//
//  miniaudio_async_test.h
//  audio_engine
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 03.09.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

#include <lockfree_container/lockfree_container.h>

namespace oo::audio {

    class miniaudio_async_test {
    public:

        miniaudio_async_test();

        ~miniaudio_async_test();

    public:

        void start();

        void stop();

    private:

        std::unique_ptr<std::thread> m_service_thread;
        mutable std::mutex m_mutex;
        std::condition_variable m_condition;
        std::atomic<bool> m_is_done{false};

        oo::lockfree_container<char> m_queue;

    };

}