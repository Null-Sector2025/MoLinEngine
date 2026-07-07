#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace MoLin {
namespace RPG {

class Inventory {
public:
    Inventory() = default;
    ~Inventory() = default;

    // 添加/移除物品
    void AddItem(const std::string& id, int count = 1);
    void RemoveItem(const std::string& id, int count = 1);
    int GetItemCount(const std::string& id) const;
    
    // 获取所有物品
    const std::unordered_map<std::string, int>& GetItems() const { return m_items; }
    
    // 清空
    void Clear();
    
    // 检查是否有物品
    bool HasItem(const std::string& id, int count = 1) const;
    
    // 获取物品总数
    int GetTotalCount() const;

private:
    std::unordered_map<std::string, int> m_items;
};

} // namespace RPG
} // namespace MoLin
