#
#  CMakeLists.txt
#  audio_engine
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 14.08.2021.
#  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
#

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(audio_engine
        VERSION 0.3.0
        LANGUAGES C CXX)

#

if (${CMAKE_SYSTEM_NAME} MATCHES "iOS")
    set(IOS TRUE)
endif ()

###########
# Options #
###########

option(AUDIO_ENGINE_MP3 "Enable the built-in MP3 decoder" ON)
option(AUDIO_ENGINE_WAV "Enable the built-in WAV decoder and encoder" OFF)
option(AUDIO_ENGINE_FLAC "Enable the built-in FLAC decoder" OFF)

################
# Dependencies #
################

include(FetchContent)

#

FetchContent_Declare(
        lockfree_container
        GIT_REPOSITORY https://github.com/ondesly/lockfree_container.git
        GIT_TAG v1.1.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE)

FetchContent_Declare(
        miniaudio
        GIT_REPOSITORY https://github.com/ondesly/miniaudio.git
        GIT_TAG v0.10.41
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE)

FetchContent_MakeAvailable(lockfree_container miniaudio)

##########
# Source #
##########

add_subdirectory(src)

###########
# Testing #
###########

enable_testing()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif ()