#include "UI/Modifier/CustomModifierVisualsController.hpp"
#include "Core/CustomModifier.hpp"
#include "HMUI/SwitchView.hpp"
#include "custom-types/shared/delegate.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/GameObject.hpp"
#include "HMUI/HoverTextSetter.hpp"
#include "UnityEngine/Transform.hpp"
#include <string>

DEFINE_TYPE(ModifiersCoreQuest, CustomModifierVisualsController);

namespace ModifiersCoreQuest {
    void CustomModifierVisualsController::Awake(){
        this->_toggle = this->GetComponent<HMUI::ToggleWithCallbacks*>();
        this->_modifierToggle = this->GetComponent<GlobalNamespace::GameplayModifierToggle*>();
        this->_background = this->transform->Find("BG")->GetComponent<HMUI::ImageView*>();
        this->DestroyImmediate(this->GetComponent<HMUI::SwitchView*>());
        this->_toggle->onValueChanged->AddListener(custom_types::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>((std::function<void(bool)>)[this] (bool state) {
            this->HandleToggleStateChanged(state);
        }));
        this->_toggle->add_stateDidChangeEvent(custom_types::MakeDelegate<System::Action_1<HMUI::ToggleWithCallbacks::SelectionState>*>((std::function<void(HMUI::ToggleWithCallbacks::SelectionState)>)[this] (HMUI::ToggleWithCallbacks::SelectionState state) {
            this->HandleToggleSelectionStateChanged(state);
        }));
    }
    void CustomModifierVisualsController::OnDestroy() {
        // HACK uhm is that correct? How do we remove just one?
        this->_toggle->onValueChanged->RemoveAllListeners();
        this->_toggle->stateDidChangeEvent = NULL;
    }

    void CustomModifierVisualsController::SetModifier(CustomModifier modifier){
        auto color = modifier.Multiplier > 0 ? positiveColor : negativeColor;
        this->_backgroundColor = modifier.Color.value_or(color);
        this->_multiplierColor = modifier.MultiplierColor.value_or(color);
        RefreshVisuals();
        RefreshText(modifier);
    }

    void CustomModifierVisualsController::RefreshText(CustomModifier modifier){
        auto multiplier = modifier.Multiplier;
        auto positiveMul = multiplier > 0.0;
        auto multiplierStr = (positiveMul ? "+" : "") + std::to_string(multiplier*100) + "%";
        auto multiplierText = _modifierToggle->_multiplierText;
        //
        multiplierText->gameObject->SetActive(static_cast<int>(multiplier*100) != 0);
        multiplierText->text = multiplierStr;
        multiplierText->color = positiveMul ? _modifierToggle->_positiveColor : multiplierText->color;
        //
        _modifierToggle->_nameText->text = modifier.Name;
        _modifierToggle->_hoverTextSetter->text = modifier.Description;
        _modifierToggle->_icon->sprite = modifier.Icon;
    }

    void CustomModifierVisualsController::RefreshColors(bool on, bool highlighted){
        //first color
        UnityEngine::Color firstColor;
        if (on) {
            firstColor = this->_backgroundColor;
        } else if (highlighted) {
            firstColor = highlightedColor;
        } else {
            firstColor = UnityEngine::Color::get_black();
        }
        //second color alpha
        float alpha;
        if (on) {
            alpha = highlighted ? 1 : highlightedAlpha;
        } else {
            alpha = highlighted ? highlightedAlpha : 1;
        }
        //applying elements
        auto elementsAlpha = on || highlighted ? 1 : secondaryAlpha;
        auto white = UnityEngine::Color::get_white();
        UnityEngine::Color elementsColor = {white.r, white.b, white.g, elementsAlpha};
        this->_modifierToggle->_icon->color = elementsColor;
        this->_modifierToggle->_nameText->color = elementsColor;
        this->_modifierToggle->_multiplierText->color = on ? white : this->_multiplierColor;
        //applying background
        this->_background->color0 = firstColor;
        firstColor.a = alpha;
        this->_background->color1 = firstColor;
    }

    void CustomModifierVisualsController::RefreshVisuals(){
        this->RefreshColors(this->_toggle->isOn, this->_toggle->selectionState == HMUI::ToggleWithCallbacks::SelectionState::Highlighted);
    }

    void CustomModifierVisualsController::HandleToggleStateChanged(bool state){
        this->RefreshVisuals();
    }

    void CustomModifierVisualsController::HandleToggleSelectionStateChanged(HMUI::ToggleWithCallbacks::SelectionState state){
        this->RefreshVisuals();
    }
}