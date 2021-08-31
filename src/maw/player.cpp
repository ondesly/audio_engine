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
    m_queue.emplace(player::command::preload, path);
}

void maw::player::release(const std::string &path) {
    m_queue.emplace(player::command::release, path);
}

void maw::player::play(const std::string &path) {
    m_queue.emplace(player::command::play, path);
}

void maw::player::stop(const std::string &path) {
    m_queue.emplace(player::command::stop, path);
}

void maw::player::reset(const std::string &path) {
    m_queue.emplace(player::command::reset, path);
}

maw::player::~player() {
    m_queue.set_done();
    m_service_thread->join();
}

void maw::player::run_service_thread() {
    m_service_thread = std::make_unique<std::thread>([&]() {
        maw::device device{[this](float *output, uint32_t frame_count, uint32_t channel_count) {
            device_callback(output, frame_count, channel_count);
        }};

        while (!m_queue.is_done()) {
            const auto command = m_queue.pop();
            const auto &path = command.second;

            switch (command.first) {
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
    });
}

void maw::player::device_callback(float *output, uint32_t frame_count, uint32_t channel_count) {
    m_callback_buf.resize(frame_count * channel_count);
    bool end = true;

    std::lock_guard<std::mutex> lock(m_playing_mutex);

    for (const auto &[path, decoder]: m_playing) {
        const auto read = decoder->read(output, m_callback_buf.data(), frame_count, channel_count);

        if (read < frame_count) {
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
    const auto decoder = std::make_shared<maw::decoder>();

    if (!decoder->init(path)) {
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
    std::lock_guard<std::mutex> lock(m_playing_mutex);

    m_playing.erase(path);
    m_decoders.erase(path);
}

void maw::player::play(maw::device &device, const std::string &path) {
    if (m_decoders.find(path) == m_decoders.end()) {
        preload(device, path);
    }

    if (!device.is_inited()) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_playing_mutex);

    if (!device.is_started()) {
        if (!device.start()) {
            return;
        }
    }

    const auto &decoder = m_decoders[path];
    m_playing.emplace(path, decoder);
}

void maw::player::stop(maw::device &device, const std::string &path) {
    if (path.empty()) {
        if (!device.is_stopped()) {
            device.stop();
        }
    } else {
        std::lock_guard<std::mutex> lock(m_playing_mutex);

        m_playing.erase(path);
    }
}

void maw::player::reset(maw::device &device, const std::string &path) {
    const auto &decoder = m_decoders[path];
    decoder->seek(0);
}
