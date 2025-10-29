cmake_minimum_required(VERSION 3.16)

project(MistralCppLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

# --------------------------
# 1. Библиотека MistralCpp
# --------------------------

include_directories(include)

set(HEADER_LIB
    include/mistralcpp.h
)

set(SOURCE_LIB
    src/mistralcpp.cpp
)

add_library(${PROJECT_NAME} SHARED 
    ${HEADER_LIB}
    ${SOURCE_LIB}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        Qt6::Core 
        Qt6::Network
)

target_compile_definitions(${PROJECT_NAME} PRIVATE MISTRALCPP_LIBRARY)

# --------------------------
# 2. Исполняемый файл для теста
# --------------------------
add_executable(MistralCppTest src/main.cpp)

target_link_libraries(MistralCppTest
    PRIVATE Qt6::Core Qt6::Network ${PROJECT_NAME}
)
