#pragma once

#include "Core/CustomModifier.hpp"
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/ToggleWithCallbacks.hpp"
#include "GlobalNamespace/GameplayModifierToggle.hpp"
#include "UnityEngine/Color.hpp"

DECLARE_CLASS_CODEGEN(ModifiersCoreQuest, CustomModifierVisualsController, UnityEngine::MonoBehaviour,
    DECLARE_DEFAULT_CTOR();
    DECLARE_SIMPLE_DTOR(); 
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, _background);
    DECLARE_INSTANCE_FIELD(HMUI::ToggleWithCallbacks*, _toggle);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::GameplayModifierToggle*, _modifierToggle);

    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, OnDestroy);

    public:
        void SetModifier(CustomModifier modifier);
    private:
        const UnityEngine::Color negativeColor = {1, 0.35, 0, 0};
        const UnityEngine::Color positiveColor = {0, 0.75, 1, 0};
        const UnityEngine::Color highlightedColor = {0.61, 0.59, 0.67, 0};
        const float highlightedAlpha = 0.5f;
        const float secondaryAlpha = 0.75f;

        UnityEngine::Color _backgroundColor;
        UnityEngine::Color _multiplierColor;

        void RefreshText(CustomModifier modifier);
        void RefreshColors(bool on, bool highlighted);
        void RefreshVisuals();
        void HandleToggleStateChanged(bool state);
        void HandleToggleSelectionStateChanged(HMUI::ToggleWithCallbacks::SelectionState state);
)