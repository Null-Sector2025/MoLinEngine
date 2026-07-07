#include "rpg/inventory.h"
#include "rpg/item_prototype.h"
#include <algorithm>

namespace RPG {

Inventory::Inventory() {}

Inventory::~Inventory() {}

void Inventory::AddItem(const std::string& id, int count) {
    // 实现添加道具
    auto it = m_items.find(id);
    if (it != m_items.end()) {
        it->second += count;
    } else {
        m_items[id] = count;
    }
}

void Inventory::RemoveItem(const std::string& id, int count) {
    auto it = m_items.find(id);
    if (it != m_items.end()) {
        it->second -= count;
        if (it->second <= 0) {
            m_items.erase(it);
        }
    }
}

int Inventory::GetItemCount(const std::string& id) const {
    auto it = m_items.find(id);
    return it != m_items.end() ? it->second : 0;
}

ItemPrototype* Inventory::GetItemPrototype(const std::string& id) {
    // TODO: 从全局道具表中查找原型
    return nullptr;
}

void Inventory::Clear() {
    m_items.clear();
}

} // namespace RPG
