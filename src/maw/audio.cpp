//
//  audio.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 19.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <memory>

#include "maw/controller.h"

#include "maw/audio.h"

namespace {

    std::unique_ptr<oo::controller> s_controller;

}

oo::audio::audio() {
    if (!s_controller) {
        s_controller = std::make_unique<oo::controller>();
    }
}

void oo::audio::load(const std::string &path) { // NOLINT(readability-convert-member-functions-to-static)
    s_controller->load_async(path);
}

void oo::audio::play(const std::string &path) { // NOLINT(readability-convert-member-functions-to-static)
    s_controller->play_async(path);
}

void oo::audio::stop(const std::string &path) { // NOLINT(readability-convert-member-functions-to-static)
    s_controller->stop_async(path);
}