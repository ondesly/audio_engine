//
//  maw.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 15.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include "maw/controller.h"

#include "maw/maw.h"

oo::maw::maw() {
    run_service_thread();
}

void oo::maw::load(const std::string &path) {
    m_queue.push({controller::command::load, path});
}

void oo::maw::play(const std::string &path) {
    m_queue.push({controller::command::play, path});
}

void oo::maw::stop(const std::string &path) {
    m_queue.push({controller::command::stop, path});
}

oo::maw::~maw() {
    m_queue.set_done();
    m_service_thread->join();
}

void oo::maw::run_service_thread() {
    m_service_thread = std::make_unique<std::thread>([&]() {
        controller controller{[&](controller::command command, const std::string &path) {
            m_queue.push({command, path});
        }};
        while (m_queue.wait()) {
            const auto command = m_queue.pop();

            switch (command.first) {
                case controller::command::load:
                    controller.load(command.second);
                    break;
                case controller::command::play:
                    controller.play(command.second);
                    break;
                case controller::command::stop:
                    controller.stop(command.second);
                    break;
                case controller::command::reset:
                    controller.reset(command.second);
                    break;
            }
        }
    });
}
