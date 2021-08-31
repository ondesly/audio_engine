//
//  test_preload_release.cpp
//  maw
//
//  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 31.08.2021.
//  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
//

#include <iostream>

#include <maw/player.h>

int main() {
    maw::player player{};

    std::cout << "initialized...";
    std::getchar();

    player.preload("sample4.mp3");

    std::cout << "preloaded...";
    std::getchar();

    player.release("sample4.mp3");

    std::cout << "released...";
    std::getchar();

    player.play("sample4.mp3");

    std::cout << "playing...";
    std::getchar();

    player.stop("sample4.mp3");

    std::cout << "stopped...";
    std::getchar();

    return 0;
}