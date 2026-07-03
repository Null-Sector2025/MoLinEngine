#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

namespace MoLin::RPG {

enum class QuestStatus { Inactive, Active, Completed, Failed };

struct QuestObjective {
    std::string description;
    int currentAmount = 0;
    int requiredAmount = 1;
    bool IsComplete() const { return currentAmount >= requiredAmount; }
};

class Quest {
public:
    std::string id;
    std::string title;
    std::string description;
    QuestStatus status = QuestStatus::Inactive;
    std::vector<QuestObjective> objectives;

    virtual ~Quest() = default;
    virtual void OnActivate() {}
    virtual void OnComplete() {}
    virtual void OnFail() {}
};

using QuestPtr = std::shared_ptr<Quest>;

class QuestManager {
public:
    virtual ~QuestManager() = default;
    virtual void RegisterQuest(QuestPtr quest) = 0;
    virtual void StartQuest(const std::string& id) = 0;
    virtual void UpdateObjective(const std::string& questId, int objIndex, int progress) = 0;
};

std::shared_ptr<QuestManager> CreateQuestManager();

} // namespace MoLin::RPG
