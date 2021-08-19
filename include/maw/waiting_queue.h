//
//  waiting_queue.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 18.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace oo {

    template<class T>
    class waiting_queue {
    public:

        void push(const T &t) {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_queue.push(t);
            }
            m_condition.notify_one();
        }

        T pop() {
            std::lock_guard<std::mutex> lock(m_mutex);

            const auto t = m_queue.front();
            m_queue.pop();
            return t;
        }

        bool wait() {
            std::unique_lock<std::mutex> lock(m_mutex);

            while (m_queue.empty() && !m_is_done) {
                m_condition.wait(lock);
            }

            return !m_queue.empty() || !m_is_done;
        }

        void set_done() {
            m_is_done = true;
            m_condition.notify_one();
        }

    private:

        bool m_is_done = false;

        std::queue<T> m_queue;
        mutable std::mutex m_mutex;
        std::condition_variable m_condition;

    };

}