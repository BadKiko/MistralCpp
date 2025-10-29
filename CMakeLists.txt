cmake_minimum_required(VERSION 3.16)

project(MistralCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

# --------------------------
# 1. Библиотека MistralCpp
# --------------------------
add_library(MistralCppLib SHARED
    mistralcpp.cpp
    mistralcpp.h
    MistralCpp_global.h
)

target_link_libraries(MistralCppLib
    PRIVATE Qt6::Core Qt6::Network
)

target_compile_definitions(MistralCppLib PRIVATE MISTRALCPP_LIBRARY)

# --------------------------
# 2. Исполняемый файл для теста
# --------------------------
add_executable(MistralCppTest main.cpp)

target_link_libraries(MistralCppTest
    PRIVATE Qt6::Core Qt6::Network MistralCppLib
)
