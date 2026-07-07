#include "dialogue_tree.h"
#include <iostream>

namespace MoLin::RPG {

void DialogueTree::AddNode(const DialogueNode& node) {
    m_Nodes[node.id] = node;
}

void DialogueTree::Start(int startNodeId) {
    auto it = m_Nodes.find(startNodeId);
    if (it != m_Nodes.end()) {
        m_CurrentNode = &it->second;
        m_Active = true;
        if (m_CurrentNode->onEnter) m_CurrentNode->onEnter();
        std::cout << "[DialogueTree] Started at node " << startNodeId << std::endl;
    }
}

DialogueNode* DialogueTree::GetCurrentNode() {
    return m_Active ? m_CurrentNode : nullptr;
}

bool DialogueTree::SelectOption(int index) {
    if (!m_CurrentNode || index < 0 || index >= m_CurrentNode->options.size())
        return false;
    auto& opt = m_CurrentNode->options[index];
    if (opt.condition && !opt.condition()) return false;
    if (opt.onSelect) opt.onSelect();
    if (opt.nextNodeId < 0) {
        m_Active = false;
        if (m_CurrentNode->onExit) m_CurrentNode->onExit();
        m_CurrentNode = nullptr;
        return true;
    }
    auto it = m_Nodes.find(opt.nextNodeId);
    if (it != m_Nodes.end()) {
        if (m_CurrentNode->onExit) m_CurrentNode->onExit();
        m_CurrentNode = &it->second;
        if (m_CurrentNode->onEnter) m_CurrentNode->onEnter();
    } else {
        m_Active = false;
        m_CurrentNode = nullptr;
    }
    return true;
}

} // namespace MoLin::RPG
