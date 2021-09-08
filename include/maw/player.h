//
//  player.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 15.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <lockfree_container/lockfree_container.h>

#include "concurrent/lockfree_set.h"

namespace maw {

    class decoder;

    class device;

    class player {
    public:

        player();

        ~player();

    public:

        void preload(const std::string &path);

        void release(const std::string &path);

        void play(const std::string &path);

        void stop(const std::string &path = "");

        void reset(const std::string &path);

    private:

        enum class command {
            preload,
            release,
            play,
            stop,
            reset
        };

    private:

        std::unique_ptr<std::thread> m_service_thread;
        mutable std::mutex m_mutex;
        std::condition_variable m_condition;
        std::atomic<bool> m_is_done{false};

        oo::lockfree_container<std::pair<player::command, std::string>> m_queue;

        std::unordered_map<std::string, std::shared_ptr<maw::decoder>> m_decoders;
        oo::lockfree_set<std::shared_ptr<maw::decoder>> m_playing;

        std::vector<float> m_callback_buf;

    private:

        void queue_command(player::command command, const std::string &path);

        void run_service_thread();

        void device_callback(float *output, uint32_t frame_count, uint32_t channel_count);

        void preload(maw::device &device, const std::string &path);

        void release(maw::device &device, const std::string &path);

        void play(maw::device &device, const std::string &path);

        void stop(maw::device &device, const std::string &path);

        void reset(maw::device &device, const std::string &path);

    };

}