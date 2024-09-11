#include "Core/Modifier.hpp"
#include <vector>
#include "Core/ModifiersManager.hpp"
#include "GlobalNamespace/GameplayModifiersPanelController.hpp"
#include "GlobalNamespace/RankModel.hpp"
#include "UI/Panel/ModifiersCorePanel.hpp"

namespace ModifiersCoreQuest {
    class GameplayModifiersPanelPatch {
        // Static class
        GameplayModifiersPanelPatch() = delete;

        public:
            static SafePtrUnity<ModifiersCorePanel> CorePanel;
            static void AddEnabledModifiers(UnityW<GlobalNamespace::GameplayModifiersPanelController> panel, std::vector<Modifier>& modifiers);
            static float GetTotalMultiplier(UnityW<GlobalNamespace::GameplayModifiersModelSO> model, std::vector<Modifier> modifiers, float energy);
            static GlobalNamespace::RankModel::Rank MaxRankForModifiers(UnityW<GlobalNamespace::GameplayModifiersModelSO> gameplayModifiersModel, std::vector<Modifier> modifiers, float energy);
    };
}