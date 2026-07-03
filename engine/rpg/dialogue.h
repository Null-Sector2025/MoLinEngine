#pragma once
#include <string>
#include <vector>

namespace MoLin::RPG {

struct DialogueLine {
    std::string speaker;
    std::string text;
};

class DialogueManager {
public:
    virtual ~DialogueManager() = default;

    // 由游戏实现，引擎只提供接口
    virtual void StartDialogue(const std::vector<DialogueLine>& lines) = 0;
    virtual void ShowNextLine() = 0;
    virtual bool IsActive() const = 0;
};

} // namespace MoLin::RPG
