#include "dialogue.h"
#include <iostream>

namespace MoLin::RPG {

class DefaultDialogueManager : public DialogueManager {
public:
    void StartDialogue(const std::vector<DialogueLine>& lines) override {
        m_Lines = lines;
        m_CurrentIndex = 0;
        m_Active = true;
    }
    void ShowNextLine() override {
        if (m_CurrentIndex + 1 < m_Lines.size()) {
            ++m_CurrentIndex;
        } else {
            m_Active = false;
        }
    }
    bool IsActive() const override { return m_Active; }
    DialogueLine GetCurrentLine() const {
        if (m_Active && m_CurrentIndex < m_Lines.size())
            return m_Lines[m_CurrentIndex];
        return {"", ""};
    }
private:
    std::vector<DialogueLine> m_Lines;
    int m_CurrentIndex = 0;
    bool m_Active = false;
};

// 为引擎提供一个创建默认对话管理器的工厂函数
std::shared_ptr<DialogueManager> CreateDialogueManager() {
    return std::make_shared<DefaultDialogueManager>();
}

} // namespace MoLin::RPG
