//
//  lockfree_set.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 05.09.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <mutex>
#include <unordered_set>

#include "concurrent/spinlock.h"

namespace oo {

    template<class T>
    class lockfree_set {
    public:

        typename std::unordered_set<T>::const_iterator begin() const {
            return m_data.begin();
        }

        typename std::unordered_set<T>::const_iterator end() const {
            return m_data.end();
        }

        void insert(const T &t) {
            std::lock_guard<spinlock> lock(m_lock);

            m_to_insert.insert(t);
            m_to_erase.erase(t);
        }

        void erase(const T &t) {
            std::lock_guard<spinlock> lock(m_lock);

            m_to_erase.insert(t);
            m_to_insert.erase(t);
        }

        void sync() {
            if (m_lock.try_lock()) {
                if (!m_to_insert.empty()) {
                    m_data.insert(m_to_insert.begin(), m_to_insert.end());
                    m_to_insert.clear();
                }

                if (!m_to_erase.empty()) {
                    for (auto it = m_to_erase.begin(); it != m_to_erase.end(); ++it) {
                        m_data.erase(*it);
                    }
                    m_to_erase.clear();
                }

                m_lock.unlock();
            }
        }

    private:

        oo::spinlock m_lock;

        std::unordered_set<T> m_data;

        std::unordered_set<T> m_to_insert;
        std::unordered_set<T> m_to_erase;

    };

}