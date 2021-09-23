#
#  CMakeLists.txt
#  audio_engine
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 16.08.2021.
#  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
#

project(tests)

#

function(add name)
    add_executable(${name} ${name}.cpp)

    target_link_libraries(${name} PRIVATE
            oo::audio_engine)

    set_target_properties(${name} PROPERTIES
            CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)

    add_test(NAME ${name} COMMAND ${name})
endfunction()

#

file(COPY ${CMAKE_CURRENT_LIST_DIR}/../res/sample.mp3 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/../res/sample2.mp3 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/../res/sample3.mp3 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_LIST_DIR}/../res/sample4.mp3 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add("test_simple")
add("test_multiple")
add("test_preload_release")