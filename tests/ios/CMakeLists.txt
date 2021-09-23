#
#  CMakeLists.txt
#  audio_engine
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 20.08.2021.
#  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
#

set(TEST_NAME test_ios)

project(${TEST_NAME})

#

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules/)

include(PlatformDefinitions)
include(DefaultProperties)
include(Dependencies)

#

set(SRC_HEADERS)
set(SRC_SOURCE)
set(APP_RES)

list(APPEND SRC_HEADERS
        AppDelegate.h
        SceneDelegate.h
        ViewController.h)

list(APPEND SRC_SOURCE
        AppDelegate.m
        main.m
        SceneDelegate.m
        ViewController.mm)

list(APPEND APP_RES
        Assets.xcassets
        LaunchScreen.storyboard
        Main.storyboard
        ../res/sample4.mp3)

list(APPEND SRC_SOURCE
        ${APP_RES})

#

add_executable(${TEST_NAME}
        ${SRC_SOURCE}
        ${SRC_HEADERS})

target_link_system_libraries(${TEST_NAME})
target_link_libraries(${TEST_NAME} PRIVATE
        oo::audio_engine)

target_include_directories(${TEST_NAME}
        PRIVATE ios)

#

set_target_properties(${TEST_NAME} PROPERTIES
        C_STANDARD 11 C_STANDARD_REQUIRED YES
        CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)

set_target_default_properties(${TEST_NAME})
set_target_default_properties(audio_engine)

target_compile_definitions(${TEST_NAME} PUBLIC $<$<CONFIG:Debug>:DEBUG=1>)

#

set_target_properties(${TEST_NAME} PROPERTIES
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.github.ondesly.audio_engine.test"
        XCODE_ATTRIBUTE_MARKETING_VERSION ${CMAKE_PROJECT_VERSION}

        RESOURCE "${APP_RES}"
        XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
        XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_GLOBAL_ACCENT_COLOR_NAME "AccentColor"
        XCODE_ATTRIBUTE_INFOPLIST_FILE "tests/ios/Info.plist")