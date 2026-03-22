cmake_minimum_required(VERSION 3.10)
project(Dunopia VERSION 4.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(SOURCES
    src/main.cpp
    src/Renderer.cpp
    src/Entity.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/QuizManager.cpp
    src/Level.cpp
    src/Particle.cpp
    src/GameEngine.cpp
    src/HUD.cpp
    src/TriviaOverlay.cpp
)

add_executable(dunopia ${SOURCES})
target_include_directories(dunopia PRIVATE include)
target_link_libraries(dunopia sfml-graphics sfml-window sfml-system)
