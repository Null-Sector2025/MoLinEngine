#include "npc.h"

namespace MoLin::RPG {

NPC::NPC(const std::string& name, SDL_Texture* texture)
    : AnimatedSprite(name, texture) {}

} // namespace MoLin::RPG
