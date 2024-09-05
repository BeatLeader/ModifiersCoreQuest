#include "UI/Modifier/ModifierPanelSpawner.hpp"

#include "Core/CustomModifier.hpp"
#include "UI/Modifier/ModifierPanelBase.hpp"
#include "Utils/ModifierUtils.hpp"
#include "Core/ModifiersManager.hpp"
#include "UI/Modifier/ModifierPanel.hpp"
#include "UnityEngine/GameObject.hpp"

DEFINE_TYPE(ModifiersCoreQuest, ModifierPanelSpawner);

namespace ModifiersCoreQuest {
    UnityW<UnityEngine::RectTransform> ModifierPanelSpawner::get_ModifiersSection() {
        return this->_patcher->ModifiersSection;
    }

    void ModifierPanelSpawner::Awake() {
        this->_patcher = this->GetComponent<ModifiersCoreQuest::ModifiersCoreUIPatcher*>();
        this->_modifierPrefab = this->get_ModifiersSection()->GetComponentInChildren<GlobalNamespace::GameplayModifierToggle*>()->gameObject;
        this->LoadExistingModifiers();
    }

    void ModifierPanelSpawner::LoadExistingModifiers() {
        auto toggles = this->_patcher->ModifiersSection->GetComponentsInChildren<GlobalNamespace::GameplayModifierToggle*>();
        for(auto toggle : toggles){
            auto key = toggle->gameplayModifier->modifierNameLocalizationKey;
            auto id = ModifiersCoreQuest::ModifierUtils::GetBaseModifierIdBySerializedName(key);
            auto modifier = ModifiersManager::AllModifiers[id];
            //
            auto panel = toggle->gameObject->AddComponent<ModifiersCoreQuest::ModifierPanel*>();
            panel->SetModifier(modifier);
            this->_baseGamePanels.emplace(id, panel);
            this->_spawnedPanels.emplace(id, panel);
        }
    }

    UnityW<ModifiersCoreQuest::ModifierPanelBase> ModifierPanelSpawner::GetSpawnedPanel(std::string id) {
        return this->_spawnedPanels[id];
    }

    UnityW<ModifiersCoreQuest::ModifierPanelBase> ModifierPanelSpawner::SpawnPanel(CustomModifier modifier) {
        ModifiersCoreQuest::CustomModifierPanel* panel;
        if (!_pooledPanels.empty()) {
            panel = _pooledPanels.top();
            _pooledPanels.pop();
            panel->gameObject->SetActive(true);
        }
        else {
            auto go = UnityEngine::Object::Instantiate(this->_modifierPrefab, this->get_ModifiersSection(), false);
            panel = go->AddComponent<ModifiersCoreQuest::CustomModifierPanel*>();
        }
        panel->SetModifier(modifier);
        this->_spawnedPanels.insert_or_assign(modifier.Id, panel);
        return panel;
    }

    void ModifierPanelSpawner::DespawnPanel(std::string id) {
        if(this->_baseGamePanels.contains(id)){
            return;
        }

        auto panel = this->_spawnedPanels[id].cast<CustomModifierPanel>();
        panel->gameObject->SetActive(true);
        this->_spawnedPanels.erase(id);
        this->_pooledPanels.push(panel);
    }
}