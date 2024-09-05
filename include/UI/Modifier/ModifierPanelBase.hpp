#pragma once

#include "Core/Modifier.hpp"
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include <functional>
#include <optional>

DECLARE_CLASS_CODEGEN(ModifiersCoreQuest, ModifierPanelBase, UnityEngine::MonoBehaviour, 
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, _toggle);
    DECLARE_INSTANCE_METHOD(void, Awake);

    public:
        virtual Modifier get_Modifier() = 0;
        std::optional<std::function<void(UnityW<ModifiersCoreQuest::ModifierPanelBase>, bool)>> ModifierStateChangedEvent;
        void SetModifierActive(bool active);
    protected:
        virtual void AwakeCS();
        virtual void HandleToggleStateChanged(bool state);
)