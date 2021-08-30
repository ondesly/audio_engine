#
#  CMakeLists.txt
#  maw
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 15.08.2021.
#  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
#

set(MAW_SRC
        maw/device.cpp)

if (${CMAKE_SYSTEM_NAME} MATCHES "iOS")
    list(APPEND MAW_SRC
            maw/controller.mm
            maw/decoder.cpp)
elseif (ANDROID)
    list(APPEND MAW_SRC
            maw/controller.cpp
            maw/decoder_android.cpp)
else ()
    list(APPEND MAW_SRC
            maw/controller.cpp
            maw/decoder.cpp)
endif ()

add_library(maw STATIC
        ${MAW_SRC})

#

target_link_libraries(maw PRIVATE miniaudio)
target_link_libraries(maw PUBLIC oo::blocking_queue)

target_compile_definitions(maw PRIVATE
        $<$<NOT:$<BOOL:${MAW_MP3}>>:MA_NO_MP3>
        $<$<NOT:$<BOOL:${MA_NO_WAV}>>:MA_NO_WAV>
        $<$<NOT:$<BOOL:${MA_NO_WAV}>>:MA_NO_FLAC>)

#

set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/../include)

set(PUBLIC_HEADERS
        ${INCLUDE_DIR}/maw/blocking_queue.h
        ${INCLUDE_DIR}/maw/controller.h)

set_target_properties(maw PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

target_include_directories(maw PUBLIC $<BUILD_INTERFACE:${INCLUDE_DIR}> $<INSTALL_INTERFACE:include>)

#

set_target_properties(maw PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)