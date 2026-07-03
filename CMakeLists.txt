cmake_minimum_required(VERSION 3.10)
project(MoLinEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找依赖
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(Lua REQUIRED)

# 引擎核心库
add_library(MoLinEngine STATIC
    engine/core/engine.cpp
    engine/graphics/renderer.cpp
    engine/scene/scene_node.cpp
    engine/script/lua_bind.cpp
    # 后续逐步添加各模块源文件
)

target_include_directories(MoLinEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
)

target_link_libraries(MoLinEngine
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${LUA_LIBRARIES}
)

# 测试（可选）
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
