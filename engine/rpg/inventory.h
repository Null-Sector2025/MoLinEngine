#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

namespace MoLin::RPG {

enum class ItemType { Consumable, Weapon, Armor, KeyItem, Misc };

struct Item {
    std::string id;
    std::string name;
    std::string description;
    ItemType type = ItemType::Misc;
    int maxStack = 99;
    int buyPrice = 0;
    int sellPrice = 0;
    // 使用效果回调
    std::function<void(class Character&)> onUse;
};

class Inventory {
public:
    void AddItem(const std::string& itemId, int count = 1);
    bool RemoveItem(const std::string& itemId, int count = 1);
    int GetItemCount(const std::string& itemId) const;
    bool HasItem(const std::string& itemId, int count = 1) const;
    void Clear();
    std::vector<std::pair<std::string, int>> GetAllItems() const;

    static void RegisterItem(const Item& item);
    static const Item* GetItemPrototype(const std::string& id);

private:
    std::unordered_map<std::string, int> m_Items;
    static std::unordered_map<std::string, Item> s_ItemDB;
};

} // namespace MoLin::RPG
