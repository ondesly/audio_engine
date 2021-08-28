//
//  test_simple.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 16.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <iostream>

#include <maw/maw.h>

int main() {
    oo::maw audio{};

    std::cout << "initialized...";
    std::getchar();

    audio.load_async("sample.mp3");

    std::cout << "loaded...";
    std::getchar();

    audio.play_async("sample.mp3");

    std::cout << "playing...";
    std::getchar();

    audio.stop_async("sample.mp3");

    std::cout << "stopped...";
    std::getchar();

    return 0;
}