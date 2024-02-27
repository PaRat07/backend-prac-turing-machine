cmake_minimum_required(VERSION 3.22)
project(prac-turing-machine)

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
        src/inoutput_field.cpp
        src/rounded_rectangle.cpp
        src/window_manager.cpp)

target_link_libraries(prac PRIVATE sfml-graphics)