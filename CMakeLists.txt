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
        main.cpp
        mainwidget.cpp
        RenderEngine.cpp
        game/Drawable.cpp
        game/Drawable.h
        game/Object.cpp
        game/Object.h
        game/Unit.cpp
        game/Unit.h
        game/MetaAttachable.cpp
        game/MetaAttachable.h
        game/MetaUnit.cpp
        game/MetaUnit.h
        game/MetaEffect.cpp
        game/MetaEffect.h
        game/MetaTurret.cpp
        game/MetaTurret.h
        game/MetaObject.cpp
        game/MetaObject.h
        game/Configs.cpp
        game/Configs.h
        game/MetaProjectiles.cpp
        game/MetaProjectiles.h
        game/Turret.cpp
        game/Turret.h
        game/Game.cpp
        game/Game.h
        game/Attachable.cpp
        game/Attachable.h
        game/PhysicsEngine.cpp
        game/PhysicsEngine.h
        game/Projectile.cpp
        game/Projectile.h
        game/SpaceEngine.cpp
        game/SpaceEngine.h
        game/Body.cpp
        game/Body.h
        game/Grid.h
)
target_link_libraries(untitled1
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::OpenGL
        Qt::OpenGLWidgets
)

