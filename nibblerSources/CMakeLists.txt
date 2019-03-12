cmake_minimum_required(VERSION 3.12)
set(NAME nibbler)
project(${NAME})
ADD_DEFINITIONS("-std=c99")

add_executable(${NAME}
        srcs/component/CollisionComponent.cpp
        srcs/component/CollisionComponent.hpp
        srcs/component/FollowComponent.cpp
        srcs/component/FollowComponent.hpp
        srcs/component/JoystickComponent.cpp
        srcs/component/JoystickComponent.hpp
        srcs/component/MotionComponent.cpp
        srcs/component/MotionComponent.hpp
        srcs/component/PositionComponent.cpp
        srcs/component/PositionComponent.hpp
        srcs/component/SpriteComponent.cpp
        srcs/component/SpriteComponent.hpp
        srcs/cores/main.cpp
        srcs/events/NextFrame.cpp
        srcs/events/NextFrame.hpp
        srcs/events/StartEvent.cpp
        srcs/events/StartEvent.hpp
        srcs/events/FoodEat.cpp
        srcs/events/FoodEat.hpp
        srcs/events/JoystickEvent.cpp
        srcs/events/JoystickEvent.hpp
        srcs/events/FoodCreation.hpp
        srcs/events/FoodCreation.cpp
        srcs/factory/Factory.cpp
        srcs/factory/Factory.hpp
        srcs/gui/widget/AWidget.cpp
        srcs/gui/widget/AWidget.hpp
        srcs/gui/widget/WidgetChat.cpp
        srcs/gui/widget/WidgetChat.hpp
        srcs/gui/widget/WidgetExit.cpp
        srcs/gui/widget/WidgetExit.hpp
        srcs/gui/widget/WidgetLobby.cpp
        srcs/gui/widget/WidgetLobby.hpp
        srcs/gui/widget/WidgetSnake.cpp
        srcs/gui/widget/WidgetSnake.hpp
        srcs/gui/widget/WidgetOption.cpp
        srcs/gui/widget/WidgetOption.hpp
        srcs/gui/Gui.cpp
        srcs/gui/Gui.hpp
        srcs/cores/Univers.cpp
        srcs/cores/Univers.hpp
        srcs/systems/FoodEatSystem.cpp
        srcs/systems/FoodEatSystem.hpp
        srcs/systems/CollisionSystem.cpp
        srcs/systems/CollisionSystem.hpp
        srcs/systems/FollowSystem.cpp
        srcs/systems/FollowSystem.hpp
        srcs/systems/FoodCreationSystem.cpp
        srcs/systems/FoodCreationSystem.hpp
        srcs/systems/JoystickSystem.cpp
        srcs/systems/JoystickSystem.hpp
        srcs/systems/MotionSystem.cpp
        srcs/systems/MotionSystem.hpp
        srcs/systems/RenderSystem.cpp
        srcs/systems/RenderSystem.hpp
        srcs/cores/Univers.cpp
        srcs/cores/Univers.hpp
        srcs/ia/Bobby.cpp
        srcs/ia/Bobby.hpp
        srcs/ia/KStar.cpp
        srcs/ia/KStar.hpp

        incs/IDisplay.hpp
        incs/Display2D.hpp
        incs/ISound.hpp
        incs/IWidget.hpp
        incs/nibbler.hpp
        srcs/KNW/IOManager.cpp
        srcs/KNW/IOManager.hpp
        srcs/KNW/BaseDataType.cpp
        srcs/KNW/BaseDataType.hpp
        srcs/KNW/ClientTCP.cpp
        srcs/KNW/ClientTCP.hpp
        srcs/KNW/Config.hpp
        srcs/KNW/DataTCP.cpp
        srcs/KNW/DataTCP.hpp
        srcs/KNW/IOTCP.cpp
        srcs/KNW/IOTCP.hpp
        srcs/KNW/ServerTCP.cpp
        srcs/KNW/ServerTCP.hpp
        srcs/network/Data.hpp
        srcs/network/SnakeClient.cpp
        srcs/network/SnakeClient.hpp
        srcs/network/SnakeServer.cpp
        srcs/network/SnakeServer.hpp
        srcs/systems/SpriteSystem.cpp
        srcs/systems/SpriteSystem.hpp
        srcs/gui/widget/WidgetConnect.cpp
        srcs/gui/widget/WidgetConnect.hpp
        srcs/KINU/Component.cpp
        srcs/KINU/Component.hpp
        srcs/KINU/Entity.cpp
        srcs/KINU/Entity.hpp
        srcs/KINU/EventsManager.cpp
        srcs/KINU/EventsManager.hpp
        srcs/KINU/Pool.hpp
        srcs/KINU/SystemsManager.cpp
        srcs/KINU/SystemsManager.hpp
        srcs/KINU/World.cpp
        srcs/KINU/World.hpp
        srcs/KINU/Config.hpp
        srcs/ia/Bobby.cpp
        srcs/ia/Bobby.hpp
        incs/Sprite.hpp
        srcs/ia/KStar.cpp
        srcs/ia/KStar.hpp
        incs/MutantGrid.tpp
        srcs/gui/widget/SpriteColorProperties.hpp
        srcs/cores/Snake.hpp
        srcs/cores/Snake.cpp
        srcs/cores/GameManager.cpp srcs/cores/GameManager.hpp srcs/dynamicLinkLibrary/DisplayDynamicLibrary.cpp srcs/dynamicLinkLibrary/DisplayDynamicLibrary.hpp srcs/dynamicLinkLibrary/SoundDynamicLibrary.cpp srcs/dynamicLinkLibrary/SoundDynamicLibrary.hpp srcs/cores/Test.cpp srcs/cores/Test.hpp srcs/gui/widget/WidgetSettingGame.cpp srcs/gui/widget/WidgetSettingGame.hpp srcs/gui/widget/WidgetServerPannel.cpp srcs/gui/widget/WidgetServerPannel.hpp srcs/gui/widget/WidgetEtat.cpp srcs/gui/widget/WidgetEtat.hpp srcs/gui/GuiData.hpp srcs/dynamicLinkLibrary/ADynamicLinkLibrary.cpp srcs/dynamicLinkLibrary/ADynamicLinkLibrary.hpp srcs/dynamicLinkLibrary/DisplayDynamicLibrary.cpp srcs/dynamicLinkLibrary/DisplayDynamicLibrary.hpp srcs/dynamicLinkLibrary/SoundDynamicLibrary.cpp srcs/dynamicLinkLibrary/SoundDynamicLibrary.hpp)


target_include_directories(
        ${NAME} PUBLIC
        srcs
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
