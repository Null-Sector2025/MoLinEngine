cmake_minimum_required(VERSION 3.15)
project(MoLinEngine VERSION 1.3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED sdl2-image)
pkg_check_modules(SDL2_MIXER REQUIRED sdl2-mixer)
pkg_check_modules(SDL2_TTF REQUIRED sdl2-ttf)
pkg_check_modules(LUA REQUIRED lua5.4)

file(GLOB_RECURSE ENGINE_SOURCES "engine/*.cpp")
list(REMOVE_ITEM ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/engine/main_entry.cpp")

add_library(MoLinEngineObj OBJECT ${ENGINE_SOURCES})
target_include_directories(MoLinEngineObj PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
)

add_library(MoLinEngine SHARED $<TARGET_OBJECTS:MoLinEngineObj>)
target_link_libraries(MoLinEngine
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${LUA_LIBRARIES}
    -lopenal -lGLESv2 -lEGL -lpthread -ldl
)

add_executable(MoLinEngineExe engine/main_entry.cpp $<TARGET_OBJECTS:MoLinEngineObj>)
target_link_libraries(MoLinEngineExe
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${LUA_LIBRARIES}
    -lopenal -lGLESv2 -lEGL -lpthread -ldl
)

install(TARGETS MoLinEngine MoLinEngineExe
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY engine/ DESTINATION include/MoLinEngine FILES_MATCHING PATTERN "*.h")