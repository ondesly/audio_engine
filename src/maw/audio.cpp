//
//  audio.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 19.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <memory>

#include "maw/maw.h"

#include "maw/audio.h"

namespace {

    std::unique_ptr<oo::maw> s_maw;

}

oo::audio::audio() {
    if (!s_maw) {
        s_maw = std::make_unique<oo::maw>();
    }
}

void oo::audio::load(const std::string &path) { // NOLINT(readability-convert-member-functions-to-static)
    s_maw->load(path);
}

void oo::audio::play(const std::string &path) { // NOLINT(readability-convert-member-functions-to-static)
    s_maw->play(path);
}

void oo::audio::stop(const std::string &path) { // NOLINT(readability-convert-member-functions-to-static)
    s_maw->stop(path);
}