#include "UI/Modifier/ModifierPanelSpawner.hpp"

#include "Core/CustomModifier.hpp"
#include "UI/Modifier/CustomModifierPanel.hpp"
#include "UI/Modifier/ModifierPanelBase.hpp"
#include "Utils/ModifierUtils.hpp"
#include "Core/ModifiersManager.hpp"
#include "UI/Modifier/ModifierPanel.hpp"
#include "UnityEngine/GameObject.hpp"
#include "logger.hpp"

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
            auto modifier = ModifiersManager::AllModifiers.at(id);
            //
            ModifierPanel* panel = new ModifiersCoreQuest::ModifierPanel(toggle->gameObject);
            panel->SetModifier(modifier);
            panel->_modifierToggle = toggle;
            this->_baseGamePanels.insert_or_assign(id, panel);
            this->_spawnedPanels.insert_or_assign(id, panel);
        }
    }

    ModifiersCoreQuest::ModifierPanelBase& ModifierPanelSpawner::GetSpawnedPanel(std::string id) {
        return *this->_spawnedPanels.at(id);
    }

    ModifiersCoreQuest::ModifierPanelBase* ModifierPanelSpawner::SpawnPanel(CustomModifier modifier) {
        ModifiersCoreQuest::CustomModifierPanel* panel;
        if (!_pooledPanels.empty()) {
            panel = _pooledPanels.top();
            _pooledPanels.pop();
            panel->gameObject->SetActive(true);
        }
        else {
            auto go = UnityEngine::Object::Instantiate(this->_modifierPrefab, this->get_ModifiersSection(), false);
            panel = new ModifiersCoreQuest::CustomModifierPanel(go);
        }
        panel->SetModifier(modifier);
        this->_spawnedPanels.insert_or_assign(modifier.Id, panel);
        return panel;
    }

    void ModifierPanelSpawner::DespawnPanel(std::string id) {
        if(this->_baseGamePanels.contains(id)){
            return;
        }

        auto* panel = dynamic_cast<ModifiersCoreQuest::CustomModifierPanel*>(this->_spawnedPanels.at(id));
        panel->gameObject->SetActive(true);
        this->_spawnedPanels.erase(id);
        this->_pooledPanels.push(panel);
    }
}