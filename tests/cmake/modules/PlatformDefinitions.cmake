#
#  PlatformDefinitions.cmake
#  maw
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 20.08.2021.
#  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
#

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

if (${CMAKE_SYSTEM_NAME} MATCHES "iOS")
    set(IOS TRUE)
endif ()