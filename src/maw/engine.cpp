//
//  engine.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 15.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#define MINIAUDIO_IMPLEMENTATION

#include <miniaudio.h>

#include "maw/decoder.h"
#include "maw/device.h"

#include "maw/engine.h"

maw::engine::engine() {
    run_service_thread();
}

void maw::engine::preload(const std::string &path) {
    queue_command(engine::command::preload, path);
}

void maw::engine::release(const std::string &path) {
    queue_command(engine::command::release, path);
}

void maw::engine::play(const std::string &path) {
    queue_command(engine::command::play, path);
}

void maw::engine::stop(const std::string &path) {
    queue_command(engine::command::stop, path);
}

void maw::engine::reset(const std::string &path) {
    queue_command(engine::command::reset, path);
}

maw::engine::~engine() {
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        m_is_done = true;
        m_condition.notify_all();
    }

    m_service_thread->join();
}

void maw::engine::queue_command(maw::engine::command command, const std::string &path) {
    m_queue.push({command, path});
    m_condition.notify_one();
}

void maw::engine::run_service_thread() {
    m_service_thread = std::make_unique<std::thread>([&]() {
        maw::device device{[this](float *output, uint32_t frame_count, uint32_t channel_count) {
            return device_callback(output, frame_count, channel_count);
        }};

        std::pair<engine::command, std::string> command;
        while (true) {
            if (m_queue.pop(command)) {
                process_command(device, command.first, command.second);
            } else {
                std::unique_lock<std::mutex> lock(m_mutex);
                if (m_is_done) {
                    break;
                } else {
                    using namespace std::chrono_literals;
                    m_condition.wait_for(lock, 100ms);
                }
            }
        }
    });
}

uint64_t maw::engine::device_callback(float *output, uint32_t frame_count, uint32_t channel_count) {
    m_callback_buf.resize(frame_count * channel_count);
    ma_uint64 total_read = 0;

    for (const auto &decoder: m_playing) {
        const auto read = decoder->read(output, m_callback_buf.data(), frame_count, channel_count);

        total_read = std::max(total_read, read);

        if (read < frame_count) {
            const auto &path = decoder->get_path();

            stop(path);
            reset(path);
        }
    }

    if (total_read == 0) {
        stop();
    }

    return total_read;
}

void maw::engine::process_command(maw::device &device, engine::command command, const std::string &path) {
    switch (command) {
        case command::preload:
            preload(device, path);
            break;
        case command::release:
            release(device, path);
            break;
        case command::play:
            play(device, path);
            break;
        case command::stop:
            stop(device, path);
            break;
        case command::reset:
            reset(device, path);
            break;
    }
}

void maw::engine::preload(maw::device &device, const std::string &path) {
    const auto decoder = std::make_shared<maw::decoder>(path);

    if (!decoder->init()) {
        return;
    }

    if (!device.is_inited()) {
        if (!device.init(decoder->get_output_format(),
                         decoder->get_output_channels(),
                         decoder->get_output_sample_rate())) {
            return;
        }
    }

    m_decoders.emplace(path, decoder);
}

void maw::engine::release(maw::device &device, const std::string &path) {
    auto decoder_it = m_decoders.find(path);
    if (decoder_it == m_decoders.end()) {
        return;
    }

    m_playing.erase(decoder_it->second);
    m_decoders.erase(decoder_it);
}

void maw::engine::play(maw::device &device, const std::string &path) {
    if (m_decoders.find(path) == m_decoders.end()) {
        preload(device, path);
    }

    if (!device.is_inited()) {
        return;
    }

    //

    auto decoder_it = m_decoders.find(path);
    if (decoder_it == m_decoders.end()) {
        return;
    }

    m_playing.insert(decoder_it->second);

    //

    if (!device.is_started()) {
        if (!device.start()) {
            return;
        }
    }
}

void maw::engine::stop(maw::device &device, const std::string &path) {
    if (path.empty()) {
        if (!device.is_stopped()) {
            device.stop();
        }
    } else {
        auto decoder_it = m_decoders.find(path);
        if (decoder_it == m_decoders.end()) {
            return;
        }

        m_playing.erase(decoder_it->second);
    }
}

void maw::engine::reset(maw::device &device, const std::string &path) {
    auto decoder_it = m_decoders.find(path);
    if (decoder_it == m_decoders.end()) {
        return;
    }

    decoder_it->second->seek(0);
}
