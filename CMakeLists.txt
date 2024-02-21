cmake_minimum_required(VERSION 3.22)
project(prac-turing-machine)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(prac
        main.cpp
        turing_machine.cpp
        tape.cpp
        table.cpp
        window.cpp
        button.cpp
        center_positioned_string.cpp
        general_data.cpp
        inoutput_field.cpp
        rounded_rectangle.cpp
        window_manager.cpp)

target_link_libraries(prac PRIVATE sfml-graphics)