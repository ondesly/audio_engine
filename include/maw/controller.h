//
//  controller.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 25.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

typedef struct ma_device ma_device;
typedef struct ma_decoder ma_decoder;

namespace oo {

    class controller {
    public:

        enum class command {
            load,
            play,
            stop,
            reset
        };

    public:

        const std::function<void(oo::controller::command, const std::string &)> m_command_fn;
        std::unordered_map<std::string, ma_decoder *> m_playing;

    public:

        explicit controller(std::function<void(command, const std::string &)> fn);

        ~controller();

    public:

        bool load(const std::string &path);

        bool play(const std::string &path);

        bool stop(const std::string &path);

        bool reset(const std::string &path);

    private:

        std::unique_ptr<ma_device> m_device;

        std::unordered_map<std::string, ma_decoder *> m_decoders;

    };

}
