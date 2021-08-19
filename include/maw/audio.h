//
//  audio.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 19.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <string>

namespace oo {

    class audio {
    public:

        audio();

    public:

        void load(const std::string &path);

        void play(const std::string &path);

        void stop(const std::string &path);

    };

}