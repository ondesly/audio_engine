//
//  test_audio.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 19.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <iostream>

#include <maw/audio.h>

int main() {
    {
        oo::audio audio{};
        audio.load("sample.mp3");

        std::cout << "loaded...";
        std::getchar();
    }

    {
        oo::audio audio{};
        audio.play("sample.mp3");

        std::cout << "playing...";
        std::getchar();
    }

    {
        oo::audio audio{};
        audio.stop("sample.mp3");

        std::cout << "stopped...";
        std::getchar();
    }

    return 0;
}