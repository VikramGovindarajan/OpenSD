cmake_minimum_required(VERSION 3.10)

project(OpenSD)

set(COOLPROP_STATIC_LIBRARY true)
SET(EXENAME opensd)

add_subdirectory ("${CMAKE_SOURCE_DIR}/externals/CoolProp")

add_executable (${EXENAME} src/main.cpp)

target_link_libraries(${EXENAME} CoolProp)

# install(TARGETS ${EXENAME})
