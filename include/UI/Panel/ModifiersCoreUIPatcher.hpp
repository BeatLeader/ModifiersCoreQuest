#pragma once

#include "custom-types/shared/macros.hpp"

#include "UnityEngine/MonoBehaviour.hpp"
#include "GlobalNamespace/GameplayModifiersPanelController.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "HMUI/TextSegmentedControl.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UI/Panel/ManualScrollView.hpp"
#include "Zenject/DiContainer.hpp"

DECLARE_CLASS_CODEGEN(ModifiersCoreQuest, ModifiersCoreUIPatcher, UnityEngine::MonoBehaviour) {
    DECLARE_DEFAULT_CTOR();
    DECLARE_SIMPLE_DTOR();
    DECLARE_INSTANCE_FIELD(GlobalNamespace::GameplayModifiersPanelController*, Panel);
    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, ModifiersSection);
    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, CustomModifiersContainer);
    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, CustomModifiersSection);
    DECLARE_INSTANCE_FIELD(HMUI::TextSegmentedControl*, TabSelector);
    DECLARE_INSTANCE_FIELD(ModifiersCoreQuest::ManualScrollView*, _scrollView);
    DECLARE_INSTANCE_FIELD(UnityEngine::RectTransform*, _scrollbarRect);

    DECLARE_INSTANCE_METHOD(void, Awake);

    private:
        void PatchUI();
        void CreateTabSelector(HMUI::TextSegmentedControl* original, Zenject::DiContainer* container);
        void CreateCustomSection(GlobalNamespace::GameplaySetupViewController* setupViewController, Zenject::DiContainer* container);
        void AddScrollbar(GlobalNamespace::GameplaySetupViewController* setupViewController);
};
