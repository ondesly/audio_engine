//
//  blocking_queue.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 18.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>

namespace oo {

    template<class T>
    class blocking_queue {
    public:

        void push(const T &t) {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_queue.push(t);
            }
            m_condition.notify_one();
        }

        template<typename... Args>
        void emplace(Args &&... args) {
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                m_queue.template emplace(std::forward<Args>(args)...);
            }
            m_condition.notify_one();
        }

        T pop() {
            std::lock_guard<std::mutex> lock(m_mutex);

            const auto t = m_queue.front();
            m_queue.pop();
            return t;
        }

        bool is_done() {
            std::unique_lock<std::mutex> lock(m_mutex);

            while (m_queue.empty()) {
                m_condition.wait(lock);

                if (m_is_done) {
                    return true;
                }
            }

            return false;
        }

        void set_done() {
            m_is_done = true;
            m_condition.notify_one();
        }

    private:

        std::atomic<bool> m_is_done{false};

        std::queue<T> m_queue;
        mutable std::mutex m_mutex;
        std::condition_variable m_condition;

    };

}