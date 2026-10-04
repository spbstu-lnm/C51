cmake_minimum_required(VERSION 3.20.0)


project(C51 VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(C51 src/main.cpp
    src/main.h
    src/EGCDResult.h
    src/EGCDResult.cpp
    src/StringConverter.h
    src/StringConverter.cpp)
