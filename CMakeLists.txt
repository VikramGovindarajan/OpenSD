cmake_minimum_required(VERSION 3.10)

project(OpenSD)

# set(COOLPROP_STATIC_LIBRARY true)
SET(EXENAME opensd)

# add_subdirectory ("${CMAKE_SOURCE_DIR}/externals/CoolProp")

file(GLOB SOURCES "src/*.cpp")
add_executable(${EXENAME} ${SOURCES})

target_include_directories(${EXENAME} PUBLIC include)
# target_link_libraries(${EXENAME} CoolProp)

# install(TARGETS ${EXENAME})
