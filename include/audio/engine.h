//
//  engine.h
//  audio_engine
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 15.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <any>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <lockfree_container/lockfree_container.h>

namespace oo::audio {

    class decoder;

    class device;

    class engine {
    public:

        engine();

        ~engine();

    public:

        void preload(const std::string &path);

        void release(const std::string &path);

        void play(const std::string &path);

        void stop(const std::string &path = "");

        void reset(const std::string &path);

        void set_volume(float value);

    private:

        enum class command {
            preload,
            release,
            play,
            stop,
            reset,
            volume
        };

    private:

        std::unique_ptr<std::thread> m_service_thread;
        mutable std::mutex m_mutex;
        std::condition_variable m_condition;
        std::atomic<bool> m_is_done{false};

        oo::lockfree_container<std::pair<engine::command, std::any>> m_queue;

        std::unordered_map<std::string, std::shared_ptr<decoder>> m_decoders;
        oo::lockfree_container<std::shared_ptr<decoder>> m_playing;

        std::vector<float> m_callback_buf;

    private:

        void queue_command(engine::command command, const std::any &param);

        void run_service_thread();

        uint64_t device_callback(float *output, uint32_t frame_count, uint32_t channel_count);

        void process_command(device &device, engine::command command, const std::any &param);

        void preload(device &device, const std::string &path);

        void release(device &device, const std::string &path);

        void play(device &device, const std::string &path);

        void stop(device &device, const std::string &path);

        void reset(device &device, const std::string &path);

        void set_volume(device &device, float value);

    };

}