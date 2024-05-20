cmake_minimum_required(VERSION 3.28)
project(cod_fov_changer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(cod_fov_changer src/main.cpp
        src/game/Game.cpp
        src/game/Game.h
        src/main.cpp
        src/process/process.cpp
        src/process/process.h
        src/process/process.h
        src/games/games.cpp
        src/games/games.h
        src/games/games.h
        src/games/games.cpp
)
target_link_libraries(cod_fov_changer
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

