//
//  controller.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 15.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#define MINIAUDIO_IMPLEMENTATION

#include <miniaudio.h>

#include "maw/decoder.h"
#include "maw/device.h"

#include "maw/controller.h"

oo::controller::controller() {
    run_service_thread();
}

void oo::controller::load_async(const std::string &path) {
    m_queue.push({controller::command::load, path});
}

void oo::controller::play_async(const std::string &path) {
    m_queue.push({controller::command::play, path});
}

void oo::controller::stop_async(const std::string &path) {
    m_queue.push({controller::command::stop, path});
}

void oo::controller::reset_async(const std::string &path) {
    m_queue.push({controller::command::reset, path});
}

oo::controller::~controller() {
    m_queue.set_done();
    m_service_thread->join();
}

void oo::controller::run_service_thread() {
    m_service_thread = std::make_unique<std::thread>([&]() {
        oo::device device{[this](float *output, uint32_t frame_count, uint32_t channel_count) {
            device_callback(output, frame_count, channel_count);
        }};

        while (m_queue.wait()) {
            const auto command = m_queue.pop();
            const auto &path = command.second;

            switch (command.first) {
                case command::load:
                    load(device, path);
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

void oo::controller::device_callback(float *output, uint32_t frame_count, uint32_t channel_count) {
    float buf[channel_count * frame_count];
    bool end = true;

    for (const auto &[path, decoder]: m_playing) {
        const auto read = decoder->read(buf, frame_count);
        for (size_t i = 0; i < read * channel_count; ++i) {
            output[i] += buf[i];
        }

        //

        if (read < frame_count) {
            stop_async(path);
            reset_async(path);
        } else {
            end = false;
        }
    }

    if (end) {
        stop_async();
    }
}

void oo::controller::load(oo::device &device, const std::string &path) {
    const auto decoder = std::make_shared<oo::decoder>();

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

void oo::controller::play(oo::device &device, const std::string &path) {
    if (!device.is_inited()) {
        return;
    }

    if (!device.is_started()) {
        if (!device.start()) {
            return;
        }
    }

    const auto &decoder = m_decoders[path];
    m_playing.emplace(path, decoder);
}

void oo::controller::stop(oo::device &device, const std::string &path) {
    if (path.empty()) {
        if (!device.is_stopped()) {
            device.stop();
        }
    } else {
        m_playing.erase(path);
    }
}

void oo::controller::reset(oo::device &device, const std::string &path) {
    const auto &decoder = m_decoders[path];
    decoder->seek(0);
}
