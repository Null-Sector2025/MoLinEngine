#include "npc.h"

namespace MoLin::RPG {

NPC::NPC(const std::string& name, SDL_Texture* texture)
    : AnimatedSprite(name, texture) {}

void NPC::AddShopItem(const std::string& itemId, int price) {
    m_ShopItems.emplace_back(itemId, price);
}

bool NPC::BuyItem(const std::string& itemId, Inventory& playerInv, int& gold) {
    for (auto& [id, price] : m_ShopItems) {
        if (id == itemId) {
            if (gold < price) return false;
            gold -= price;
            playerInv.AddItem(itemId);
            return true;
        }
    }
    return false;
}

} // namespace MoLin::RPG
