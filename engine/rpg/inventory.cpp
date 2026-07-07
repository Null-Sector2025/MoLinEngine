#include "rpg/inventory.h"
#include <algorithm>

namespace MoLin {
namespace RPG {

void Inventory::AddItem(const std::string& id, int count) {
    if (count <= 0) return;
    m_items[id] += count;
}

void Inventory::RemoveItem(const std::string& id, int count) {
    if (count <= 0) return;
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

void Inventory::Clear() {
    m_items.clear();
}

bool Inventory::HasItem(const std::string& id, int count) const {
    return GetItemCount(id) >= count;
}

int Inventory::GetTotalCount() const {
    int total = 0;
    for (const auto& pair : m_items) {
        total += pair.second;
    }
    return total;
}

} // namespace RPG
} // namespace MoLin
