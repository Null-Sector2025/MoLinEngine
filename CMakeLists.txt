cmake_minimum_required(VERSION 3.15)
project(MoLinEngine VERSION 1.3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(PkgConfig REQUIRED)

# 宏：尝试多个包名
macro(find_pkg PKG_PREFIX PKG_NAMES)
    set(${PKG_PREFIX}_FOUND FALSE)
    foreach(PKG ${PKG_NAMES})
        if(NOT ${PKG_PREFIX}_FOUND)
            pkg_check_modules(${PKG_PREFIX} QUIET ${PKG})
            if(${PKG_PREFIX}_FOUND)
                set(${PKG_PREFIX}_PKG_NAME ${PKG})
                message(STATUS "Found ${PKG}")
                break()
            endif()
        endif()
    endforeach()
    if(NOT ${PKG_PREFIX}_FOUND)
        message(FATAL_ERROR "Could not find any of ${PKG_NAMES}")
    endif()
endmacro()

find_pkg(SDL2 "sdl2;SDL2")
find_pkg(SDL2_IMAGE "sdl2-image;SDL2_image")
find_pkg(SDL2_MIXER "sdl2-mixer;SDL2_mixer")
find_pkg(SDL2_TTF "sdl2-ttf;SDL2_ttf")
find_pkg(LUA "lua5.4;lua-5.4;lua54;lua")

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
