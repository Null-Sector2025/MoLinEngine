#include "quest.h"
#include <iostream>

namespace MoLin::RPG {

class DefaultQuestManager : public QuestManager {
public:
    void RegisterQuest(QuestPtr quest) override { m_Quests[quest->id] = quest; }
    void StartQuest(const std::string& id) override {
        auto it = m_Quests.find(id);
        if (it != m_Quests.end() && it->second->status == QuestStatus::Inactive) {
            it->second->status = QuestStatus::Active;
            it->second->OnActivate();
        }
    }
    void UpdateObjective(const std::string& questId, int objIndex, int progress) override {
        auto it = m_Quests.find(questId);
        if (it == m_Quests.end()) return;
        auto& quest = it->second;
        if (quest->status != QuestStatus::Active) return;
        if (objIndex >= 0 && objIndex < quest->objectives.size()) {
            quest->objectives[objIndex].currentAmount = progress;
            if (quest->objectives[objIndex].IsComplete()) {
                bool allDone = true;
                for (auto& obj : quest->objectives) if (!obj.IsComplete()) { allDone = false; break; }
                if (allDone) { quest->status = QuestStatus::Completed; quest->OnComplete(); }
            }
        }
    }
private:
    std::unordered_map<std::string, QuestPtr> m_Quests;
};

std::shared_ptr<QuestManager> CreateQuestManager() {
    return std::make_shared<DefaultQuestManager>();
}

} // namespace MoLin::RPG
