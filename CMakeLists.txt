cmake_minimum_required(VERSION 3.29)
project(untitled1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGL
        OpenGLWidgets
        REQUIRED)

add_executable(untitled1
        src/main.cpp
        src/ui/BattlefieldWidget.cpp
        src/game/Drawable.cpp
        src/game/Object.cpp
        src/game/Unit.cpp
        src/game/MetaAttachable.cpp
        src/game/MetaUnit.cpp
        src/game/MetaEffect.cpp
        src/game/MetaTurret.cpp
        src/game/MetaObject.cpp
        src/game/Configs.cpp
        src/game/MetaProjectiles.cpp
        src/game/Turret.cpp
        src/game/Game.cpp
        src/game/Attachable.cpp
        src/game/Projectile.cpp
        src/game/GridsManager.cpp
        src/game/Sensor.cpp
        src/game/MetaDrawable.cpp
        src/game/Effect.cpp
        include/battle_widget.h
        src/ui/welcome_widget.cpp
        include/welcome_widget.h
        src/ui/splash_widget.cpp
        include/splash_widget.h
        src/ui/battle_widget.cpp
        include/battle_widget.h
        src/ui/RenderEngine.cpp
)
add_subdirectory(src/tmxlite)
#link_libraries(tmxlite)
include_directories(src/tmxlite/include)
include_directories(include)

target_link_libraries(untitled1
        tmxlite
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        Qt::OpenGLWidgets
)

