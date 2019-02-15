cmake_minimum_required(VERSION 3.12)
set(NAME nibbler)
project(${NAME})

add_executable(${NAME}
        class/component/CollisionComponent.cpp
        class/component/CollisionComponent.hpp
        class/component/FollowComponent.cpp
        class/component/FollowComponent.hpp
        class/component/JoystickComponent.cpp
        class/component/JoystickComponent.hpp
        class/component/MotionComponent.cpp
        class/component/MotionComponent.hpp
        class/component/PositionComponent.cpp
        class/component/PositionComponent.hpp
        class/component/SpriteComponent.cpp
        class/component/SpriteComponent.hpp
        class/cores/main.cpp
        class/events/NextFrame.cpp
        class/events/NextFrame.hpp
        class/events/StartEvent.cpp
        class/events/StartEvent.hpp
        class/events/FoodEat.cpp
        class/events/FoodEat.hpp
        class/events/JoystickEvent.cpp
        class/events/JoystickEvent.hpp
        class/events/FoodCreation.hpp
        class/events/FoodCreation.cpp
        class/factory/Factory.cpp
        class/factory/Factory.hpp
        class/gui/widget/AWidget.cpp
        class/gui/widget/AWidget.hpp
        class/gui/widget/WidgetChat.cpp
        class/gui/widget/WidgetChat.hpp
        class/gui/widget/WidgetExit.cpp
        class/gui/widget/WidgetExit.hpp
        class/gui/widget/WidgetLobby.cpp
        class/gui/widget/WidgetLobby.hpp
        class/gui/widget/WidgetSnake.cpp
        class/gui/widget/WidgetSnake.hpp
        class/gui/widget/WidgetOption.cpp
        class/gui/widget/WidgetOption.hpp
        class/gui/Gui.cpp
        class/gui/Gui.hpp
        class/cores/Univers.cpp
        class/cores/Univers.hpp
        class/logger/display/logger_debug.cpp
        class/logger/display/logger_error.cpp
        class/logger/display/logger_fatal.cpp
        class/logger/display/logger_info.cpp
        class/logger/display/logger_success.cpp
        class/logger/display/logger_trace.cpp
        class/logger/display/logger_warn.cpp
        class/logger/logger/logger_init.cpp
        class/logger/utils/logger_get_time.cpp
        class/logger/utils/logger_init_open_file.cpp
#        class/network/ClientTCP.cpp
#        class/network/ClientTCP.hpp
#        class/network/ServerTCP.cpp
#        class/network/ServerTCP.hpp
#        class/network/IGameNetwork.hpp
        class/systems/FoodEatSystem.cpp
        class/systems/FoodEatSystem.hpp
        class/systems/CollisionSystem.cpp
        class/systems/CollisionSystem.hpp
        class/systems/FollowSystem.cpp
        class/systems/FollowSystem.hpp
        class/systems/FoodCreationSystem.cpp
        class/systems/FoodCreationSystem.hpp
        class/systems/JoystickSystem.cpp
        class/systems/JoystickSystem.hpp
        class/systems/MotionSystem.cpp
        class/systems/MotionSystem.hpp
        class/systems/RenderSystem.cpp
        class/systems/RenderSystem.hpp
        class/cores/Univers.cpp
        class/cores/Univers.hpp
        class/ia/Bobby.cpp
        class/ia/Bobby.hpp
#        class/ia/AStar.cpp
#        class/ia/AStar.hpp
        class/ia/KStar.cpp
        class/ia/KStar.hpp

        incs/IDisplay.hpp
        incs/Display2D.hpp
        incs/ISound.hpp
        incs/IWidget.hpp
        incs/logger.h
        incs/logger_utils.h
        incs/nibbler.hpp
        class/KNW/IOManager.cpp
        class/KNW/IOManager.hpp
        class/KNW/BaseDataType.cpp
        class/KNW/BaseDataType.hpp
        class/KNW/ClientTCP.cpp
        class/KNW/ClientTCP.hpp
        class/KNW/Config.hpp
        class/KNW/DataTCP.cpp
        class/KNW/DataTCP.hpp
        class/KNW/IOTCP.cpp
        class/KNW/IOTCP.hpp
        class/KNW/ServerTCP.cpp
        class/KNW/ServerTCP.hpp
        class/network/Data.hpp
        class/network/SnakeClient.cpp
        class/network/SnakeClient.hpp
        class/network/SnakeServer.cpp
        class/network/SnakeServer.hpp
        class/systems/SpriteSystem.cpp
        class/systems/SpriteSystem.hpp
        class/gui/widget/WidgetConnect.cpp
        class/gui/widget/WidgetConnect.hpp
        class/KINU/Component.cpp
        class/KINU/Component.hpp
        class/KINU/Entity.cpp
        class/KINU/Entity.hpp
        class/KINU/EventsManager.cpp
        class/KINU/EventsManager.hpp
        class/KINU/Pool.hpp
        class/KINU/SystemsManager.cpp
        class/KINU/SystemsManager.hpp
        class/KINU/World.cpp
        class/KINU/World.hpp
        class/KINU/Config.hpp
        class/ia/Bobby.cpp
        class/ia/Bobby.hpp
        class/gui/widget/WidgetMassiveButton.cpp
        class/gui/widget/WidgetMassiveButton.hpp
        incs/Sprite.hpp
        class/ia/KStar.cpp
        class/ia/KStar.hpp
        incs/MutantGrid.tpp
        class/gui/widget/SpriteColorProperties.hpp
        class/cores/Snake.hpp
        class/cores/Snake.cpp
        class/cores/GameManagement.cpp class/cores/GameManagement.hpp class/cores/ExternalLibraryManager.cpp class/cores/ExternalLibraryManager.hpp)


target_include_directories(
        ${NAME} PUBLIC
        class
        incs
        $ENV{HOME}/.brew/include
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/extern/imgui/
        ${CMAKE_SOURCE_DIR}/extern/imgui/imgui/
        ${CMAKE_SOURCE_DIR}/extern/imgui/imgui-sfml/
        ${CMAKE_SOURCE_DIR}/dynamicLibraries/commun/
)

set_target_properties(${NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

target_link_libraries(${NAME} PRIVATE Boost::filesystem)
target_link_libraries(${NAME} PRIVATE Boost::thread)
target_link_libraries(${NAME} PRIVATE Boost::program_options)
target_link_libraries(${NAME} PRIVATE ${SFML_LIBRARIES})
target_link_libraries(${NAME} PRIVATE imgui-sfml)
target_link_libraries(${NAME} PRIVATE ${OPENGL_LIBRARIES})
