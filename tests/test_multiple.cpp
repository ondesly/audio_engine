//
//  test_multiple.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 19.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <string>
#include <thread>
#include <vector>

#include <maw/audio.h>

int main() {
    std::vector<std::string> paths{
            "sample.mp3",
            "sample2.mp3",
            "sample3.mp3",
            "sample4.mp3"
    };

    oo::audio audio{};
    for (const auto &path : paths) {
        audio.load(path);
    }

    srand(time(nullptr)); // NOLINT(cert-msc51-cpp)
    std::thread t([&]() {
        for (size_t i = 0; i < 100; ++i) {
            oo::audio audio{};
            if (rand() % 3 == 0) { // NOLINT(cert-msc50-cpp)
                audio.stop(paths[rand() % 4]); // NOLINT(cert-msc50-cpp)
            } else {
                audio.play(paths[rand() % 4]); // NOLINT(cert-msc50-cpp)
            }
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(300ms);
        }
    });
    t.join();

    return 0;
}