//
//  test_preload_release.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 31.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <iostream>

#include <maw/engine.h>

int main() {
    maw::engine engine{};

    std::cout << "initialized...";
    std::getchar();

    engine.preload("sample4.mp3");

    std::cout << "preloaded...";
    std::getchar();

    engine.release("sample4.mp3");

    std::cout << "released...";
    std::getchar();

    engine.play("sample4.mp3");

    std::cout << "playing...";
    std::getchar();

    engine.stop("sample4.mp3");

    std::cout << "stopped...";
    std::getchar();

    return 0;
}