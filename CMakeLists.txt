cmake_minimum_required(VERSION 3.22)
project(prac-turing-machine)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(prac
        main.cpp
        src/turing_machine.cpp
        src/tape.cpp
        src/table.cpp
        src/window.cpp
        src/button.cpp
        src/center_positioned_string.cpp
        src/general_data.cpp
        src/window_manager.cpp
        elements/tape_head.h
        src/tape_head.cpp
        app/colors.h
        src/colors.cpp
        src/inoutput_field.cpp)

target_link_libraries(prac PRIVATE sfml-graphics)