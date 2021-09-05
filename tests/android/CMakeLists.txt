#
#  CMakeLists.txt
#  maw
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 20.08.2021.
#  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
#

set(TEST_NAME test_android)

project(${TEST_NAME})

#

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules/)

include(PlatformDefinitions)
include(DefaultProperties)
include(Dependencies)

#

set(SRC_SOURCE)

list(APPEND SRC_SOURCE
        app/src/main/cpp/a_audio_test.cpp
        app/src/main/cpp/log_buffer.cpp
        app/src/main/cpp/main.cpp
        app/src/main/cpp/miniaudio_async_test.cpp
        app/src/main/cpp/miniaudio_test.cpp)

#

add_library(${TEST_NAME} SHARED ${SRC_SOURCE})

target_link_system_libraries(${TEST_NAME})
target_link_libraries(${TEST_NAME} PRIVATE
        maw miniaudio)

#

set_target_properties(${TEST_NAME} PROPERTIES
        C_STANDARD 11 C_STANDARD_REQUIRED YES
        CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)

set_target_default_properties(${TEST_NAME})
set_target_default_properties(maw)

target_compile_definitions(${TEST_NAME} PUBLIC $<$<CONFIG:Debug>:DEBUG=1>)

#

file(COPY ${CMAKE_CURRENT_LIST_DIR}/../res/sample4.mp3 DESTINATION ${CMAKE_CURRENT_LIST_DIR}/app/src/main/assets)