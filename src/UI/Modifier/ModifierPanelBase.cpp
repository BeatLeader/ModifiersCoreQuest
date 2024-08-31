#include "include/UI/Modifier/ModifierPanelBase.hpp"
#include "custom-types/shared/delegate.hpp"
#include <functional>

DEFINE_TYPE(ModifiersCoreQuest, ModifierPanelBase);

namespace ModifiersCoreQuest {
    // CT Methods
    void ModifierPanelBase::SetModifierActive(bool active){
        this->_toggle->isOn = active;
    }
    void ModifierPanelBase::Awake(){
        this->AwakeCS();
    }
    void ModifierPanelBase::HandleToggleStateChanged(bool state){
        if(this->ModifierStateChangedEvent.has_value()){
            this->ModifierStateChangedEvent.value()(this, state);
        }
    }

    // C++ Virtuals to emulate C# virtual
    void ModifierPanelBase::AwakeCS(){
        this->_toggle = this->GetComponent<UnityEngine::UI::Toggle*>();
        this->_toggle->onValueChanged->AddListener(custom_types::MakeDelegate<UnityEngine::Events::UnityAction_1<bool>*>((std::function<void(bool)>)[this] (bool state) {
            this->HandleToggleStateChanged(state);
        }));
    }
}