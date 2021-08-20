#!/bin/sh

cd "$(dirname "$0")"/../.. || exit
mkdir -p cmake-build-ios

cmake -S . -B cmake-build-ios -G Xcode -DCMAKE_SYSTEM_NAME=iOS -DBUILD_TESTING=ON