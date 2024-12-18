cmake_minimum_required(VERSION 3.29)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif ()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGL
        OpenGLWidgets
        Multimedia
        MultimediaWidgets
        REQUIRED)

add_executable(untitled1
        src/main.cpp
        src/game/Drawable.cpp
        src/game/Object.cpp
        src/game/Unit.cpp
        src/game/MetaAttachable.cpp
        src/game/MetaUnit.cpp
        src/game/MetaEffect.cpp
        src/game/MetaTurret.cpp
        src/game/MetaObject.cpp
        src/game/MetaProjectiles.cpp
        src/game/Turret.cpp
        src/game/Game.cpp
        src/game/Attachable.cpp
        src/game/Projectile.cpp
        src/game/GridsManager.cpp
        src/game/Sensor.cpp
        src/game/MetaDrawable.cpp
        src/game/Effect.cpp
        src/game/Arm.cpp
        src/game/MetaArm.cpp


        src/tmxlite/FreeFuncs.cpp
        src/tmxlite/ImageLayer.cpp
        src/tmxlite/Map.cpp
        src/tmxlite/Object.cpp
        src/tmxlite/ObjectGroup.cpp
        src/tmxlite/Property.cpp
        src/tmxlite/TileLayer.cpp
        src/tmxlite/LayerGroup.cpp
        src/tmxlite/Tileset.cpp
        src/tmxlite/ObjectTypes.cpp
        src/tmxlite/miniz.c
        src/tmxlite/FreeFuncs.cpp


        src/utils/pugixml.cpp

        resources.qrc

        src/game/config/UnitConfigs.cpp
        src/game/config/MapConfig.cpp
        src/game/config/GameConfig.cpp
        src/include/GameConfig.h
        src/game/Logic.cpp

        src/game/Tile.cpp
        src/game/path/PathFind.cpp
        src/game/path/FlowField.cpp
        src/game/utils.cpp
        src/game/BoidSensor.cpp
        src/game/Flock.cpp
        src/game/SimpleEffect.cpp
        src/game/Trigger.cpp
        src/game/Controller.cpp
        src/game/DirectPathFollowController.cpp
        src/game/WarfareFogManager.cpp
        src/game/MeleeSensor.cpp

        src/ui/RenderEngine.cpp
        src/ui/BattlefieldWidget.cpp

        src/ui/include/battle_widget.h
        src/ui/welcome_widget.cpp
        src/ui/include/welcome_widget.h
        src/ui/splash_widget.cpp
        src/ui/include/splash_widget.h
        src/ui/battle_widget.cpp
        src/ui/include/battle_widget.h
        src/ui/game_end_widget.cpp
        src/ui/include/game_end_widget.h
        src/ui/game_end_widget.ui
        src/ui/suspend_menu_widget.cpp
        src/ui/include/suspend_menu_widget.h
        src/ui/suspend_menu_widget.ui
        src/ui/help_slide_widget.cpp
        src/ui/include/help_slide_widget.h
        src/ui/help_slide_widget.ui
        src/ui/settings_widget.cpp
        src/ui/include/settings_widget.h
        src/ui/settings_widget.ui
        src/game/AudioManager.cpp
        src/ui/main_window.cpp
        src/ui/include/main_window.h
        src/ui/main_window.ui

        src/ui/dialog_widget.cpp
        src/ui/include/dialog_widget.h
        src/ui/dialog_widget.ui

        src/ui/choose_widget.cpp
        src/ui/include/choose_widget.h
        src/ui/choose_widget.ui
)
include_directories(src/include)
include_directories(src/ui/include)

add_subdirectory(zlib)
add_subdirectory(zstd/build/cmake/)
#find_package(ZLIB REQUIRED)
#find_package(zstd REQUIRED)
target_link_libraries(untitled1
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        Qt::OpenGLWidgets
        Qt::Multimedia
        Qt::MultimediaWidgets
        zlibstatic
        libzstd_static
)

if (CMAKE_HOST_WIN32)
    target_link_libraries(untitled1
            opengl32
            glu32
    )
endif ()
