#
#  DefaultProperties.cmake
#  maw
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 20.08.2021.
#  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
#

function(set_target_default_properties target)
    if (IOS)
        set_target_properties(${target} PROPERTIES
                MACOSX_BUNDLE TRUE
                XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Debug] YES
                XCODE_ATTRIBUTE_ONLY_ACTIVE_ARCH[variant=Release] NO
                XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Debug] dwarf
                XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT[variant=Release] dwarf-with-dsym
                XCODE_ATTRIBUTE_ENABLE_TESTABILITY[variant=Debug] YES
                XCODE_ATTRIBUTE_COPY_PHASE_STRIP NO
                XCODE_ATTRIBUTE_ALWAYS_SEARCH_USER_PATHS NO
                XCODE_ATTRIBUTE_GCC_NO_COMMON_BLOCKS YES
                XCODE_ATTRIBUTE_GCC_OPTIMIZATION_LEVEL[variant=Release] s
                XCODE_ATTRIBUTE_CLANG_ENABLE_MODULES YES
                XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
                XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_WEAK YES

                XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 13.0
                XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"
                XCODE_ATTRIBUTE_VALIDATE_PRODUCT[variant=Release] YES
                XCODE_ATTRIBUTE_ENABLE_NS_ASSERTIONS[variant=Release] NO
                XCODE_ATTRIBUTE_ENABLE_STRICT_OBJC_MSGSEND YES)
    endif ()
endfunction()