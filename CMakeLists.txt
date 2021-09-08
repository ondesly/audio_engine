#
#  CMakeLists.txt
#  maw
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 14.08.2021.
#  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
#

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(maw
        VERSION 0.1.0
        LANGUAGES C CXX)

#

if (${CMAKE_SYSTEM_NAME} MATCHES "iOS")
    set(IOS TRUE)
endif ()

###########
# Options #
###########

option(MAW_MP3 "Disable the built-in MP3 decoder" ON)
option(MAW_WAV "Disable the built-in WAV decoder and encoder" OFF)
option(MAW_FLAC "Disable the built-in FLAC decoder" OFF)

################
# Dependencies #
################

include(FetchContent)

#

FetchContent_Declare(
        lockfree_container
        GIT_REPOSITORY https://github.com/ondesly/lockfree_container.git
        GIT_TAG v1.0.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE)

FetchContent_Declare(
        miniaudio
        GIT_REPOSITORY https://github.com/ondesly/miniaudio.git
        GIT_TAG v0.10.41
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE)

FetchContent_MakeAvailable(lockfree_container miniaudio)

#########
# Build #
#########

add_subdirectory(src)

###########
# Install #
###########

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} miniaudio
        EXPORT ${PROJECT_NAME}-config
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}${INSTALL_SUFFIX}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}${INSTALL_SUFFIX}/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}-config DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

###########
# Testing #
###########

enable_testing()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif ()