//
//  test_simple.cpp
//  audio_engine
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 16.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <iostream>

#include <audio/engine.h>

int main() {
    oo::audio::engine engine{};

    std::cout << "initialized...";
    std::getchar();

    engine.preload("sample.mp3");

    std::cout << "preloaded...";
    std::getchar();

    engine.play("sample.mp3");

    std::cout << "playing...";
    std::getchar();

    engine.stop("sample.mp3");

    std::cout << "stopped...";
    std::getchar();

    return 0;
}