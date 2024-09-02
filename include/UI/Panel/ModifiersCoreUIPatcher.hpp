#pragma once

#include "custom-types/shared/macros.hpp"

#include "UnityEngine/MonoBehaviour.hpp"
#include "GlobalNamespace/GameplayModifiersPanelController.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UI/Panel/ManualScrollView.hpp"

DECLARE_CLASS_CODEGEN(ModifiersCoreQuest, ModifiersCoreUIPatcher, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD(GlobalNamespace::GameplayModifiersPanelController*, Panel);
    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, ModifiersSection);
    DECLARE_INSTANCE_FIELD(ModifiersCoreQuest::ManualScrollView*, _scrollView);
    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, _scrollbarRect);

    DECLARE_INSTANCE_METHOD(void, Awake);

    private:
        void PatchUI();
        void AddScrollbar();
        void SetupScrollbar();
)