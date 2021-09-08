//
//  player.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 15.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#define MINIAUDIO_IMPLEMENTATION

#include <miniaudio.h>

#include "maw/decoder.h"
#include "maw/device.h"

#include "maw/player.h"

maw::player::player() {
    run_service_thread();
}

void maw::player::preload(const std::string &path) {
    queue_command(player::command::preload, path);
}

void maw::player::release(const std::string &path) {
    queue_command(player::command::release, path);
}

void maw::player::play(const std::string &path) {
    queue_command(player::command::play, path);
}

void maw::player::stop(const std::string &path) {
    queue_command(player::command::stop, path);
}

void maw::player::reset(const std::string &path) {
    queue_command(player::command::reset, path);
}

maw::player::~player() {
    m_is_done = true;
    m_service_thread->join();
}

void maw::player::queue_command(maw::player::command command, const std::string &path) {
    m_queue.push({command, path});
    m_condition.notify_one();
}

void maw::player::run_service_thread() {
    m_service_thread = std::make_unique<std::thread>([&]() {
        maw::device device{[this](float *output, uint32_t frame_count, uint32_t channel_count) {
            device_callback(output, frame_count, channel_count);
        }};

        do {
            std::pair<player::command, std::string> command;
            while (m_queue.pop(command)) {
                switch (command.first) {
                    case command::preload:
                        preload(device, command.second);
                        break;
                    case command::release:
                        release(device, command.second);
                        break;
                    case command::play:
                        play(device, command.second);
                        break;
                    case command::stop:
                        stop(device, command.second);
                        break;
                    case command::reset:
                        reset(device, command.second);
                        break;
                }
            }

            std::unique_lock<std::mutex> lock(m_mutex);
            m_condition.wait(lock);
        } while (!m_is_done);
    });
}

void maw::player::device_callback(float *output, uint32_t frame_count, uint32_t channel_count) {
    m_callback_buf.resize(frame_count * channel_count);
    bool end = true;

    m_playing.sync();
    for (const auto &decoder: m_playing) {
        const auto read = decoder->read(output, m_callback_buf.data(), frame_count, channel_count);

        if (read < frame_count) {
            const auto &path = decoder->get_path();

            stop(path);
            reset(path);
        } else {
            end = false;
        }
    }

    if (end) {
        stop();
    }
}

void maw::player::preload(maw::device &device, const std::string &path) {
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

void maw::player::release(maw::device &device, const std::string &path) {
    const auto &decoder = m_decoders[path];
    m_playing.erase(decoder);

    m_decoders.erase(path);
}

void maw::player::play(maw::device &device, const std::string &path) {
    if (m_decoders.find(path) == m_decoders.end()) {
        preload(device, path);
    }

    if (!device.is_inited()) {
        return;
    }

    //

    const auto &decoder = m_decoders[path];
    m_playing.insert(decoder);

    //

    if (!device.is_started()) {
        if (!device.start()) {
            return;
        }
    }
}

void maw::player::stop(maw::device &device, const std::string &path) {
    if (path.empty()) {
        if (!device.is_stopped()) {
            device.stop();
        }
    } else {
        const auto &decoder = m_decoders[path];
        m_playing.erase(decoder);
    }
}

void maw::player::reset(maw::device &device, const std::string &path) {
    const auto &decoder = m_decoders[path];
    decoder->seek(0);
}
