#
#  Dependencies.cmake
#  audio_engine
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 20.08.2021.
#  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
#

function(target_link_system_libraries target)
    if (IOS)
        include_directories(/System/Library/Frameworks)

        find_library(AV_FOUNDATION_LIBRARY AVFoundation)
        find_library(AUDIO_TOOLBOX_LIBRARY AudioToolbox)

        target_link_libraries(${target} PRIVATE
                ${AV_FOUNDATION_LIBRARY}
                ${AUDIO_TOOLBOX_LIBRARY})
    elseif (ANDROID)
        target_link_libraries(${target} PRIVATE
                aaudio
                android
                log)
    endif ()
endfunction()