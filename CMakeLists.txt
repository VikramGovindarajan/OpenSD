cmake_minimum_required(VERSION 3.10)

project(OpenSD)

set(COOLPROP_STATIC_LIBRARY true)

add_subdirectory ("${CMAKE_SOURCE_DIR}/externals/CoolProp")

add_executable (opensd "${CMAKE_SOURCE_DIR}/main.cpp")

target_link_libraries(opensd CoolProp)
