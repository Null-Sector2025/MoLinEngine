#include "dialogue.h"

namespace MoLin::RPG {

class DefaultDialogueManager : public DialogueManager {
public:
    void StartDialogue(const std::vector<DialogueLine>& lines) override {
        m_Lines = lines; m_CurrentIndex = 0; m_Active = true;
    }
    void ShowNextLine() override {
        if (m_CurrentIndex + 1 < m_Lines.size()) ++m_CurrentIndex; else m_Active = false;
    }
    bool IsActive() const override { return m_Active; }
private:
    std::vector<DialogueLine> m_Lines;
    int m_CurrentIndex = 0;
    bool m_Active = false;
};

std::shared_ptr<DialogueManager> CreateDialogueManager() {
    return std::make_shared<DefaultDialogueManager>();
}

} // namespace MoLin::RPG
