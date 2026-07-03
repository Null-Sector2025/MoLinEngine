#pragma once
#include <string>
#include <unordered_map>

namespace MoLin::RPG {

struct Item {
    std::string id;
    std::string name;
    std::string description;
    int maxStack = 99;
};

class Inventory {
public:
    void AddItem(const std::string& itemId, int count = 1);
    bool RemoveItem(const std::string& itemId, int count = 1);
    int GetItemCount(const std::string& itemId) const;
    bool HasItem(const std::string& itemId, int count = 1) const;
    void Clear();

    // 静态物品数据库（引擎使用者需注册物品原型）
    static void RegisterItem(const Item& item);
    static const Item* GetItemPrototype(const std::string& id);

private:
    std::unordered_map<std::string, int> m_Items;
    static std::unordered_map<std::string, Item> s_ItemDB;
};

} // namespace MoLin::RPG
