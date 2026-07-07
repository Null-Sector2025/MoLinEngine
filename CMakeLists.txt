cmake_minimum_required(VERSION 3.15)
project(MoLinEngine VERSION 1.3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# =====================================================
# 平台检测与依赖查找
# =====================================================

if(WIN32)
    # Windows: 使用 vcpkg 提供的 CMake 包（不依赖 pkg-config）
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(SDL2_mixer CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
    find_package(Lua REQUIRED)
    
    set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIR})
    set(SDL2_LIBRARIES SDL2::SDL2)
    set(SDL2_IMAGE_LIBRARIES SDL2_image::SDL2_image)
    set(SDL2_MIXER_LIBRARIES SDL2_mixer::SDL2_mixer)
    set(SDL2_TTF_LIBRARIES SDL2_ttf::SDL2_ttf)
    set(LUA_LIBRARIES Lua::Lua)
    set(PLATFORM_LIBS "")
    set(PLATFORM_LIBS_EXE "")
    
elseif(APPLE)
    # macOS: 使用 Homebrew 的 pkg-config
    find_package(PkgConfig REQUIRED)
    
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
    
    # macOS 特殊链接（框架 + 库）
    set(PLATFORM_LIBS 
        -framework OpenGL
        -framework OpenAL
        -framework Cocoa
        -framework IOKit
    )
    set(PLATFORM_LIBS_EXE ${PLATFORM_LIBS})
    
else()
    # Linux / Unix: 使用 pkg-config
    find_package(PkgConfig REQUIRED)
    
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
    
    # Linux 系统库
    set(PLATFORM_LIBS -lopenal -lGLESv2 -lEGL -lpthread -ldl -lm)
    set(PLATFORM_LIBS_EXE ${PLATFORM_LIBS})
endif()

# =====================================================
# 收集所有源文件
# =====================================================

file(GLOB_RECURSE ENGINE_SOURCES "engine/*.cpp")
# 排除 main_entry.cpp（独立可执行文件）
list(REMOVE_ITEM ENGINE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/engine/main_entry.cpp")

# =====================================================
# 核心库（对象库，避免重复编译）
# =====================================================

add_library(MoLinEngineObj OBJECT ${ENGINE_SOURCES})

target_include_directories(MoLinEngineObj PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/debug/imgui
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
)

# =====================================================
# 动态库（共享库）
# =====================================================

add_library(MoLinEngine SHARED $<TARGET_OBJECTS:MoLinEngineObj>)

target_link_libraries(MoLinEngine
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${LUA_LIBRARIES}
    ${PLATFORM_LIBS}
)

# 设置动态库版本号
set_target_properties(MoLinEngine PROPERTIES
    VERSION 1.3.0
    SOVERSION 1
)

# =====================================================
# 可执行文件（包含 main_entry.cpp）
# =====================================================

add_executable(MoLinEngineExe engine/main_entry.cpp $<TARGET_OBJECTS:MoLinEngineObj>)

target_link_libraries(MoLinEngineExe
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${LUA_LIBRARIES}
    ${PLATFORM_LIBS_EXE}
)

# =====================================================
# macOS 特殊：设置动态库后缀和安装路径
# =====================================================

if(APPLE)
    set_target_properties(MoLinEngine PROPERTIES
        SUFFIX ".dylib"
        INSTALL_NAME_DIR "@rpath"
    )
endif()

# =====================================================
# 安装配置
# =====================================================

install(TARGETS MoLinEngine MoLinEngineExe
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)

install(DIRECTORY engine/ DESTINATION include/MoLinEngine
    FILES_MATCHING PATTERN "*.h"
    PATTERN "debug/imgui/*.cpp" EXCLUDE
)

# =====================================================
# 打印配置信息（便于调试）
# =====================================================

message(STATUS "========================================")
message(STATUS "MoLinEngine 配置完成")
message(STATUS "平台: ${CMAKE_SYSTEM_NAME}")
message(STATUS "C++ 标准: C++${CMAKE_CXX_STANDARD}")
message(STATUS "SDL2: ${SDL2_LIBRARIES}")
message(STATUS "Lua: ${LUA_LIBRARIES}")
message(STATUS "========================================")
