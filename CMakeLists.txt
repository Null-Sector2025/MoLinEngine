cmake_minimum_required(VERSION 3.10)
project(MoLinEngine VERSION 1.2.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Lua REQUIRED)

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

# 静态库
add_library(MoLinEngine STATIC ${ENGINE_SOURCES})
target_include_directories(MoLinEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${LUA_INCLUDE_DIR}
)
target_link_libraries(MoLinEngine
    ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${LUA_LIBRARIES}
)

# 可执行文件
add_executable(MoLinEngineExe
    engine/main_entry.cpp
    ${ENGINE_SOURCES}
)
target_include_directories(MoLinEngineExe PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
    ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${LUA_INCLUDE_DIR}
)
target_link_libraries(MoLinEngineExe
    ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${LUA_LIBRARIES}
)

install(TARGETS MoLinEngine MoLinEngineExe
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(DIRECTORY engine/ DESTINATION include/MoLinEngine FILES_MATCHING PATTERN "*.h")
