#include "UI/Modifier/ModifierPanel.hpp"
#include "Core/Modifier.hpp"
#include "UI/Modifier/ModifierPanelBase.hpp"

DEFINE_TYPE(ModifiersCoreQuest, ModifierPanel);

namespace ModifiersCoreQuest {
    Modifier ModifierPanel::get_Modifier(){
        return this->_modifier;
    }

    void ModifierPanel::SetModifier(Modifier modifier){
        this->_modifier = modifier;
    }
}