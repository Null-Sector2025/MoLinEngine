#include "game/game_state.h"
#include <fstream>
#include <iostream>

namespace MoLin {

GameState& GameState::Instance() {
    static GameState instance;
    return instance;
}

void GameState::SetFlag(const std::string& key, bool value) {
    m_flags[key] = value;
}

bool GameState::GetFlag(const std::string& key) const {
    auto it = m_flags.find(key);
    return it != m_flags.end() && it->second;
}

bool GameState::Save(int slot) {
    // TODO: 实现存档
    std::cout << "存档到槽位 " << slot << std::endl;
    return true;
}

bool GameState::Load(int slot) {
    // TODO: 实现读档
    std::cout << "从槽位 " << slot << " 读档" << std::endl;
    return true;
}

} // namespace MoLin
