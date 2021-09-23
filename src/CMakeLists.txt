#
#  CMakeLists.txt
#  audio_engine
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 15.08.2021.
#  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
#

set(AUDIO_ENGINE_SRC
        audio/device.cpp)

if (IOS)
    list(APPEND AUDIO_ENGINE_SRC
            audio/engine.mm
            audio/decoder.cpp)
elseif (ANDROID)
    list(APPEND AUDIO_ENGINE_SRC
            audio/engine.cpp
            audio/decoder_android.cpp)
else ()
    list(APPEND AUDIO_ENGINE_SRC
            audio/engine.cpp
            audio/decoder.cpp)
endif ()

add_library(audio_engine STATIC ${AUDIO_ENGINE_SRC})
add_library(oo::audio_engine ALIAS audio_engine)

#

target_link_libraries(audio_engine PRIVATE miniaudio)
target_link_libraries(audio_engine PUBLIC oo::lockfree_container)

target_compile_definitions(audio_engine PRIVATE
        $<$<NOT:$<BOOL:${AUDIO_ENGINE_MP3}>>:MA_NO_MP3>
        $<$<NOT:$<BOOL:${AUDIO_ENGINE_WAV}>>:MA_NO_WAV>
        $<$<NOT:$<BOOL:${AUDIO_ENGINE_WAV}>>:MA_NO_FLAC>)

#

target_include_directories(audio_engine PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../include>)

#

set_target_properties(audio_engine PROPERTIES
        CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)