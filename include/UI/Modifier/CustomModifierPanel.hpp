#pragma once

#include "Core/CustomModifier.hpp"
#include "Core/Modifier.hpp"
#include "ModifierPanelBase.hpp"
#include "UI/Modifier/CustomModifierVisualsController.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CUSTOM(ModifiersCoreQuest, CustomModifierPanel, ModifierPanelBase,
    DECLARE_INSTANCE_FIELD_PRIVATE(ModifiersCoreQuest::CustomModifierVisualsController*, _visualsController);
    DECLARE_INSTANCE_FIELD_PRIVATE(GlobalNamespace::GameplayModifierToggle*, _modifierToggle);

    public:
        Modifier get_Modifier();
        void SetModifier(CustomModifier customModifier);
    protected:
        void HandleToggleStateChanged(bool state);
        void AwakeCS();
    private:
        CustomModifier _modifier;
)