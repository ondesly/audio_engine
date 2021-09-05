//
//  spinlock.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 05.09.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <atomic>

namespace oo {

    class spinlock {
    public:

        spinlock() {
            m_lock.clear();
        }

        spinlock(const spinlock &) = delete;

    public:

        void lock() {
            while (m_lock.test_and_set(std::memory_order_acquire));
        }

        bool try_lock() {
            return !m_lock.test_and_set(std::memory_order_acquire);
        }

        void unlock() {
            m_lock.clear(std::memory_order_release);
        }

    private:

        std::atomic_flag m_lock{};

    };

}