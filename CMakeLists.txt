cmake_minimum_required(VERSION 3.22)
project(prac-tunring-machine)

add_subdirectory(sfml-templates)

add_executable(prac main.cpp turing_machine.cpp tape.cpp)