#pragma once
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace MoLin::RPG {

struct DialogueOption {
    std::string text;
    int nextNodeId = -1;
    std::function<bool()> condition; // 条件满足才显示
    std::function<void()> onSelect;
};

struct DialogueNode {
    int id = 0;
    std::string speaker;
    std::string text;
    std::vector<DialogueOption> options;
    std::function<void()> onEnter;
    std::function<void()> onExit;
};

class DialogueTree {
public:
    void AddNode(const DialogueNode& node);
    void Start(int startNodeId);
    DialogueNode* GetCurrentNode();
    bool SelectOption(int index);
    bool IsActive() const { return m_Active; }

private:
    std::unordered_map<int, DialogueNode> m_Nodes;
    DialogueNode* m_CurrentNode = nullptr;
    bool m_Active = false;
};

} // namespace MoLin::RPG
