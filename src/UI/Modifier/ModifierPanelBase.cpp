#include "include/UI/Modifier/ModifierPanelBase.hpp"
#include "custom-types/shared/delegate.hpp"
#include "UnityEngine/GameObject.hpp"
#include "logger.hpp"
#include <functional>
#include <unistd.h>

namespace ModifiersCoreQuest {

    void ModifierPanelBase::SetModifierActive(bool active){
        this->_toggle->isOn = active;
    }
    void ModifierPanelBase::HandleToggleStateChanged(bool state){
        if(this->ModifierStateChangedEvent.has_value()){
            this->ModifierStateChangedEvent.value()(*this, state);
        }
    }
    void ModifierPanelBase::Awake(){
        this->_toggle = this->gameObject->GetComponent<UnityEngine::UI::Toggle*>();
        this->_toggle->onValueChanged->AddListener(custom_types::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>((std::function<void(bool)>)[this] (bool state) {
            this->HandleToggleStateChanged(state);
        }));
    }
}