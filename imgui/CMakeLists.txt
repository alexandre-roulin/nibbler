file(READ imgui-sfml/imconfig-SFML.h IMGUI-SFML_CONFIG)

FILE(WRITE ${PROJECT_SOURCE_DIR}/imgui.h
        "#pragma once"
        "${IMGUI-SFML_CONFIG}")