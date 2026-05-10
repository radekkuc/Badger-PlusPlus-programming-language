cmake_minimum_required(VERSION 3.10.0)
project(Badger++ VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Badger++ main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/compiler.cpp
    src/interpreter.cpp
    src/astnodes.cpp
)

include_directories(include)