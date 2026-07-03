#include "inventory.h"
#include <iostream>

namespace MoLin::RPG {

std::unordered_map<std::string, Item> Inventory::s_ItemDB;

void Inventory::AddItem(const std::string& itemId, int count) {
    auto proto = GetItemPrototype(itemId);
    if (!proto) return;
    m_Items[itemId] += count;
    if (m_Items[itemId] > proto->maxStack) m_Items[itemId] = proto->maxStack;
}
bool Inventory::RemoveItem(const std::string& itemId, int count) {
    auto it = m_Items.find(itemId);
    if (it == m_Items.end() || it->second < count) return false;
    it->second -= count;
    if (it->second <= 0) m_Items.erase(it);
    return true;
}
int Inventory::GetItemCount(const std::string& itemId) const {
    auto it = m_Items.find(itemId);
    return (it != m_Items.end()) ? it->second : 0;
}
bool Inventory::HasItem(const std::string& itemId, int count) const {
    return GetItemCount(itemId) >= count;
}
void Inventory::Clear() { m_Items.clear(); }
std::vector<std::pair<std::string, int>> Inventory::GetAllItems() const {
    std::vector<std::pair<std::string, int>> items;
    for (auto& [id, cnt] : m_Items) items.emplace_back(id, cnt);
    return items;
}
void Inventory::RegisterItem(const Item& item) { s_ItemDB[item.id] = item; }
const Item* Inventory::GetItemPrototype(const std::string& id) {
    auto it = s_ItemDB.find(id);
    return (it != s_ItemDB.end()) ? &it->second : nullptr;
}

} // namespace MoLin::RPG
