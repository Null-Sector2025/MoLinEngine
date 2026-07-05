cmake_minimum_required(VERSION 3.10)
project(MoLinEngine VERSION 1.3.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(WIN32)
    find_package(SDL2 CONFIG REQUIRED)
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(SDL2_mixer CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

    find_package(Lua 5.4 QUIET)
    find_package(lua CONFIG QUIET)
    find_package(unofficial-lua CONFIG QUIET)

    if(TARGET Lua::Lua)
        set(LUA_TARGET Lua::Lua)
    elseif(TARGET lua::lua)
        set(LUA_TARGET lua::lua)
    elseif(TARGET unofficial::lua)
        set(LUA_TARGET unofficial::lua)
    elseif(DEFINED LUA_LIBRARIES)
        set(LUA_TARGET ${LUA_LIBRARIES})
    else()
        message(FATAL_ERROR "Lua not found.")
    endif()

    get_target_property(SDL2_INC SDL2::SDL2 INTERFACE_INCLUDE_DIRECTORIES)
    set(SDL2_INCLUDE_DIRS ${SDL2_INC})
    set(COMMON_LIBS SDL2::SDL2 SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf ${LUA_TARGET})
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    pkg_check_modules(LUA REQUIRED lua5.4)
    set(SDL2_INCLUDE_DIRS ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${LUA_INCLUDE_DIRS})
    set(COMMON_LIBS SDL2 SDL2_image SDL2_mixer SDL2_ttf lua5.4)
endif()

set(ENGINE_SOURCES
    engine/core/engine.cpp engine/core/config.cpp engine/core/timer.cpp
    engine/core/collision.cpp
    engine/scene/scene.cpp engine/input/input.cpp
    engine/script/lua_bind.cpp engine/script/lua_api.cpp
    engine/resource/resource_manager.cpp
    engine/graphics/sprite.cpp engine/graphics/camera.cpp
    engine/graphics/tilemap.cpp engine/graphics/animation.cpp
    engine/graphics/animated_sprite.cpp engine/graphics/particle_system.cpp
    engine/graphics/sprite_batch.cpp engine/graphics/text_renderer.cpp
    engine/graphics/transition.cpp
    engine/audio/audio_manager.cpp
    engine/ui/ui_button.cpp engine/ui/ui_system.cpp
    engine/crypto/simple_crypto.cpp engine/save/save_manager.cpp
    engine/game/game_state.cpp
    engine/rpg/character.cpp engine/rpg/inventory.cpp engine/rpg/npc.cpp
    engine/rpg/dialogue.cpp engine/rpg/quest.cpp engine/rpg/battle.cpp
    engine/rpg/event_system.cpp engine/rpg/map_loader.cpp engine/rpg/dialogue_tree.cpp
)

# 编辑器源文件
set(EDITOR_SOURCES
    engine/editor/editor_main.cpp
    engine/debug/imgui/imgui.cpp
    engine/debug/imgui/imgui_widgets.cpp
    engine/debug/imgui/imgui_impl_sdl2.cpp
    engine/debug/imgui/imgui_impl_sdlrenderer2.cpp
)

# 对象库（引擎核心）
add_library(MoLinEngineObj OBJECT ${ENGINE_SOURCES})
target_include_directories(MoLinEngineObj PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${SDL2_INCLUDE_DIRS}
)

add_library(MoLinEngine $<TARGET_OBJECTS:MoLinEngineObj>)
target_link_libraries(MoLinEngine ${COMMON_LIBS})

# 引擎可执行文件
add_executable(MoLinEngineExe engine/main_entry.cpp $<TARGET_OBJECTS:MoLinEngineObj>)
target_include_directories(MoLinEngineExe PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(MoLinEngineExe ${COMMON_LIBS})
if(WIN32)
    set_target_properties(MoLinEngineExe PROPERTIES WIN32_EXECUTABLE FALSE)
endif()

# 编辑器可执行文件
add_executable(MoLinEditor
    ${EDITOR_SOURCES}
    $<TARGET_OBJECTS:MoLinEngineObj>
)
target_include_directories(MoLinEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${CMAKE_CURRENT_SOURCE_DIR}/engine/debug/imgui
    ${SDL2_INCLUDE_DIRS}
)
target_link_libraries(MoLinEditor ${COMMON_LIBS})
if(WIN32)
    set_target_properties(MoLinEditor PROPERTIES WIN32_EXECUTABLE FALSE)
endif()

install(TARGETS MoLinEngine MoLinEngineExe MoLinEditor
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY engine/ DESTINATION include/MoLinEngine FILES_MATCHING PATTERN "*.h")
