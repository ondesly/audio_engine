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

#include <maw/player.h>

int main() {
    std::vector<std::string> paths{
            "sample.mp3",
            "sample2.mp3",
            "sample3.mp3",
            "sample4.mp3"
    };

    maw::player player{};
    for (const auto &path: paths) {
        player.load(path);
    }

    srand(time(nullptr)); // NOLINT(cert-msc51-cpp)
    std::thread t([&]() {
        for (size_t i = 0; i < 100; ++i) {
            if (rand() % 3 == 0) { // NOLINT(cert-msc50-cpp)
                player.stop(paths[rand() % 4]); // NOLINT(cert-msc50-cpp)
            } else {
                player.play(paths[rand() % 4]); // NOLINT(cert-msc50-cpp)
            }
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(300ms);
        }
    });
    t.join();

    return 0;
}