#pragma once
#include "../rpg/character.h"
#include <string>
#include <unordered_map>

namespace MoLin {

class GameState {
public:
    static GameState& Instance();
    
    RPG::Character& GetPlayer() { return m_player; }
    const RPG::Character& GetPlayer() const { return m_player; }
    
    void SetFlag(const std::string& key, bool value);
    bool GetFlag(const std::string& key) const;
    
    bool Save(int slot);
    bool Load(int slot);
    
private:
    GameState() = default;
    ~GameState() = default;
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;
    
    RPG::Character m_player;
    std::unordered_map<std::string, bool> m_flags;
};

} // namespace MoLin