//
//  controller.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 15.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <memory>
#include <string>
#include <thread>
#include <unordered_map>

#include "maw/waiting_queue.h"

namespace oo {

    class decoder;

    class device;

    class controller {
    public:

        controller();

        ~controller();

    public:

        void load_async(const std::string &path);

        void play_async(const std::string &path);

        void stop_async(const std::string &path = "");

        void reset_async(const std::string &path);

    private:

        enum class command {
            load,
            play,
            stop,
            reset
        };

    private:

        std::unique_ptr<std::thread> m_service_thread;
        oo::waiting_queue<std::pair<controller::command, std::string>> m_queue;

        std::unordered_map<std::string, std::shared_ptr<oo::decoder>> m_decoders;
        std::unordered_map<std::string, std::shared_ptr<oo::decoder>> m_playing;

    private:

        void run_service_thread();

        void device_callback(float *output, uint32_t frame_count, uint32_t channel_count);

        void load(oo::device &device, const std::string &path);

        void play(oo::device &device, const std::string &path);

        void stop(oo::device &device, const std::string &path);

        void reset(oo::device &device, const std::string &path);

    };

}