file(READ imgui-sfml/imconfig-SFML.h IMGUI-SFML_CONFIG)

FILE(WRITE ${CMAKE_BINARY_DIR}/imgui.h
        "#pragma once"
        "${IMGUI-SFML_CONFIG}")