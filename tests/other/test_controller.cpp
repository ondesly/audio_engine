//
//  test_controller.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 16.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <iostream>

#include <maw/controller.h>

int main() {
    oo::controller controller{};

    std::cout << "initialized...";
    std::getchar();

    controller.load_async("sample.mp3");

    std::cout << "loaded...";
    std::getchar();

    controller.play_async("sample.mp3");

    std::cout << "playing...";
    std::getchar();

    controller.stop_async("sample.mp3");

    std::cout << "stopped...";
    std::getchar();

    return 0;
}