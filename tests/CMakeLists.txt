#
#  CMakeLists.txt
#  maw
#
#  Created by Dmitrii Torkhov <dmitriitorkhov@gmail.com> on 20.08.2021.
#  Copyright © 2021 Dmitrii Torkhov. All rights reserved.
#

if (IOS)
    add_subdirectory(ios)
elseif (ANDROID)
    add_subdirectory(android)
else ()
    add_subdirectory(desktop)
endif ()