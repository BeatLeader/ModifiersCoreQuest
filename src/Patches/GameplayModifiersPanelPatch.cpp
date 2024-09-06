#include "Patches/GameplayModifiersPanelPatch.hpp"
#include "Core/Modifier.hpp"
#include "Core/ModifiersManager.hpp"
#include "GlobalNamespace/GameplayModifiersModelSO.hpp"
#include "System/Collections/Generic/Dictionary_2.hpp"
#include "GlobalNamespace/ScoreModel.hpp"
#include "hooking.hpp"
#include "UI/Panel/ModifiersCorePanel.hpp"
#include "UnityEngine/GameObject.hpp"
#include "TMPro/TextMeshPro.hpp"
#include "logger.hpp"
#include <string>
#include <vector>

namespace ModifiersCoreQuest {

    MAKE_AUTO_HOOK_MATCH(GameplayModifiersPanelControllerAwake, &GlobalNamespace::GameplayModifiersPanelController::Awake, void, GlobalNamespace::GameplayModifiersPanelController* self) {
        GameplayModifiersPanelControllerAwake(self);
        self->gameObject->AddComponent<ModifiersCoreQuest::ModifiersCorePanel*>();
        // Here because we dont have static constructors in c++ :/
        ModifiersCoreQuest::ModifiersManager::LoadBaseGameModifiers();
    }

    MAKE_AUTO_HOOK_ORIG_MATCH(GameplayModifiersPanelControllerRefreshTotalMultiplierAndRankUIHook, &GlobalNamespace::GameplayModifiersPanelController::RefreshTotalMultiplierAndRankUI, void, GlobalNamespace::GameplayModifiersPanelController* self) {
        std::vector<Modifier> modifiers = {};
        GameplayModifiersPanelPatch::AddEnabledModifiers(self, modifiers);
        auto totalMultiplier = GameplayModifiersPanelPatch::GetTotalMultiplier(self->_gameplayModifiersModel, modifiers, 1);
        auto color = totalMultiplier >= 1 ? self->_positiveColor : self->_negativeColor;
        //multiplier
        self->_totalMultiplierValueText->text = std::to_string((int)(totalMultiplier * 100)) + "%";
        self->_totalMultiplierValueText->color = color;
        // rank
        auto maxRank = GameplayModifiersPanelPatch::MaxRankForModifiers(self->_gameplayModifiersModel, modifiers, 1);
        auto rankName = GlobalNamespace::RankModel::GetRankName(maxRank);
        self->_maxRankValueText->text = rankName;
        self->_maxRankValueText->color = color;
    }

    void GameplayModifiersPanelPatch::AddEnabledModifiers(UnityW<GlobalNamespace::GameplayModifiersPanelController> panel, std::vector<Modifier>& modifiers){
        for(auto modifier : ModifiersManager::get_Modifiers()) {
            //if base game modifier
            auto it = ModifiersManager::GameplayModifierParams.find(modifier.Id);
            if (it != ModifiersManager::GameplayModifierParams.end()) {
                auto paramsSO = it->second;
                //acquiring getter
                GlobalNamespace::__GameplayModifiersModelSO__GameplayModifierBoolGetter* getter = NULL;
                panel->_gameplayModifiersModel->_gameplayModifierGetters->TryGetValue(paramsSO, getter);
                //adding if enabled
                if(getter && getter->Invoke(panel->_gameplayModifiers)) {
                    modifiers.push_back(modifier);
                }
            }
            else if (ModifiersManager::GetModifierState(modifier.Id)) {
                modifiers.push_back(modifier);
            }
        }
    }

    float GameplayModifiersPanelPatch::GetTotalMultiplier(UnityW<GlobalNamespace::GameplayModifiersModelSO> model, std::vector<Modifier> modifiers, float energy) {
        float totalMultiplier = 1;
        for(auto modifier : modifiers) {
            auto name = model->_noFailOn0Energy->modifierNameLocalizationKey;
            if (modifier.Name == name) {
                if (energy <= 9.999999747378752E-06) {
                    totalMultiplier += modifier.Multiplier;
                }
            }
            else {
                totalMultiplier += modifier.Multiplier;
            }
        }
        if (totalMultiplier < 0){
            totalMultiplier = 0;
        }
        return totalMultiplier;
    }

    GlobalNamespace::RankModel::Rank GameplayModifiersPanelPatch::MaxRankForModifiers(UnityW<GlobalNamespace::GameplayModifiersModelSO> gameplayModifiersModel, std::vector<Modifier> modifiers, float energy) {
        auto totalMultiplier = GetTotalMultiplier(gameplayModifiersModel, modifiers, energy);
        auto num = GlobalNamespace::ScoreModel::GetModifiedScoreForGameplayModifiersScoreMultiplier(1000000, totalMultiplier);
        return GlobalNamespace::RankModel::GetRankForScore(0, num, 1000000, num);
    }
}