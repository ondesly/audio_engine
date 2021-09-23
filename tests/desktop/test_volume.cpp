//
//  test_volume.cpp
//  audio_engine
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 23.09.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <iostream>

#include <audio/engine.h>

int main() {
    oo::audio::engine engine{};
    engine.set_volume(0.25F);

    engine.play("sample4.mp3");

    std::cout << "playing...";
    std::getchar();

    engine.set_volume(0.5);

    std::cout << "volume 50%";
    std::getchar();

    engine.set_volume(1.F);

    std::cout << "volume 100%";
    std::getchar();

    return 0;
}