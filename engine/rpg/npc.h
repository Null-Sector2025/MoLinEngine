#pragma once
#include <string>
#include <vector>
#include <functional>
#include "../graphics/animated_sprite.h"
#include "inventory.h"

namespace MoLin::RPG {

class NPC : public AnimatedSprite {
public:
    NPC(const std::string& name, SDL_Texture* texture);

    void SetDialogue(const std::string& text) { m_DialogueText = text; }
    std::string GetDialogue() const { return m_DialogueText; }

    void AddShopItem(const std::string& itemId, int price);
    bool BuyItem(const std::string& itemId, Inventory& playerInv, int& gold);
    const std::vector<std::pair<std::string, int>>& GetShopItems() const { return m_ShopItems; }

    std::function<void()> onInteract;

protected:
    std::string m_DialogueText;
    std::vector<std::pair<std::string, int>> m_ShopItems; // 商品ID, 价格
};

} // namespace MoLin::RPG
