#pragma once
#include <string>
#include <vector>
#include <memory>

namespace MoLin::RPG {

struct DialogueLine {
    std::string speaker;
    std::string text;
};

class DialogueManager {
public:
    virtual ~DialogueManager() = default;
    virtual void StartDialogue(const std::vector<DialogueLine>& lines) = 0;
    virtual void ShowNextLine() = 0;
    virtual bool IsActive() const = 0;
};

std::shared_ptr<DialogueManager> CreateDialogueManager();

} // namespace MoLin::RPG
