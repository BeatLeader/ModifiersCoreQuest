#include "UI/Modifier/CustomModifierPanel.hpp"

#include "Core/CustomModifier.hpp"
#include "UI/Modifier/CustomModifierVisualsController.hpp"
#include "UI/Modifier/ModifierPanelBase.hpp"
#include "UnityEngine/GameObject.hpp"
#include "Core/ModifiersManager.hpp"

namespace ModifiersCoreQuest {
    Modifier CustomModifierPanel::get_Modifier() {
        return this->_modifier;
    }

    void CustomModifierPanel::Awake() {
        ModifierPanelBase::Awake();
        this->gameObject->name = "CustomModifier";
        this->_modifierToggle = this->gameObject->GetComponent<GlobalNamespace::GameplayModifierToggle*>();
        this->_modifierToggle->enabled = false;
        this->_visualsController = this->gameObject->AddComponent<ModifiersCoreQuest::CustomModifierVisualsController*>();
    }

    void CustomModifierPanel::SetModifier(CustomModifier customModifier){
        this->_modifier = customModifier;
        this->_visualsController->SetModifier(customModifier);
        bool modifierActive = ModifiersManager::GetModifierState(customModifier.Id);
        this->SetModifierActive(modifierActive);
    }

    void CustomModifierPanel::HandleToggleStateChanged(bool state){
        ModifiersManager::SetModifierState(this->_modifier.Id, state);
        ModifierPanelBase::HandleToggleStateChanged(state); 
    }
}