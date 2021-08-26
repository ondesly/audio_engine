//
//  maw.h
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 15.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#pragma once

#include <memory>
#include <string>
#include <thread>
#include <unordered_map>

#include "maw/controller.h"
#include "maw/waiting_queue.h"

namespace oo {

    class maw {
    public:

        maw();

        ~maw();

    public:

        void load(const std::string &path);

        void play(const std::string &path);

        void stop(const std::string &path);

    private:

        std::unique_ptr<std::thread> m_service_thread;
        oo::waiting_queue<std::pair<controller::command, std::string>> m_queue;

    private:

        void run_service_thread();

    };

}