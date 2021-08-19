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

#include "maw/waiting_queue.h"

typedef struct ma_device ma_device;

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
        oo::waiting_queue<ma_device *> m_queue;

        std::unordered_map<std::string, ma_device *> m_devices;

    private:

        void run_service_thread();

    };

}