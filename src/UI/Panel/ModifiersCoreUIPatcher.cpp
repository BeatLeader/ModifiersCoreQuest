#include "UI/Panel/ModifiersCoreUIPatcher.hpp"

#include "UnityEngine/Object.hpp"
#include "Zenject/Context.hpp"
#include "HMUI/Touchable.hpp"
#include "UnityEngine/UI/RectMask2D.hpp"
#include "UI/Panel/ScrollContentSizeUpdater.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "Zenject/DiContainer.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/Vector3.hpp"
#include "HMUI/VerticalScrollIndicator.hpp"
#include "logger.hpp"

DEFINE_TYPE(ModifiersCoreQuest, ModifiersCoreUIPatcher);

namespace ModifiersCoreQuest {
    void ModifiersCoreUIPatcher::Awake() {
        this->Panel = this->GetComponent<GlobalNamespace::GameplayModifiersPanelController*>();
        this->ModifiersSection = this->transform->Find("Modifiers").cast<UnityEngine::RectTransform>();
        this->PatchUI();
    }

    void ModifiersCoreUIPatcher::PatchUI() {
        auto context = UnityEngine::Object::FindObjectsOfType<Zenject::Context*>()->Last();
        auto container = context->Container;
        //creating scroll view
        auto scrollContainer = UnityEngine::GameObject::New_ctor("ScrollContainer");
        scrollContainer->AddComponent<HMUI::Touchable*>();
        scrollContainer->AddComponent<UnityEngine::UI::RectMask2D*>();
        auto scrollRect = scrollContainer->GetComponent<UnityEngine::RectTransform*>();
        _scrollView = scrollContainer->AddComponent<ModifiersCoreQuest::ManualScrollView*>();
        //
        scrollRect->SetParent(ModifiersSection->parent, false);
        scrollRect->sizeDelta = { 100, 46 };
        scrollRect->pivot = { 0.5, 1 };
        scrollRect->localPosition = { 0, -16, 0 };
        //setting up modifiers section
        ModifiersSection->SetParent(scrollRect, false);
        ModifiersSection->anchorMin = { 0, 1 };
        ModifiersSection->anchorMax = UnityEngine::Vector2::get_one();
        ModifiersSection->sizeDelta = UnityEngine::Vector2::get_zero();
        ModifiersSection->pivot = { 0.5, 1 };
        auto contentUpdater = this->ModifiersSection->gameObject->AddComponent<ModifiersCoreQuest::ScrollContentSizeUpdater*>();
        contentUpdater->scrollView = _scrollView;
        //
        auto modifiersSizeFitter = ModifiersSection->gameObject->AddComponent<UnityEngine::UI::ContentSizeFitter*>();
        modifiersSizeFitter->verticalFit = UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize;
        //setting up scroll view
        _scrollView->_scrollViewDirection = HMUI::ScrollView::ScrollViewDirection::Vertical;
        _scrollView->_scrollType = HMUI::ScrollView::ScrollType::FixedCellSize;
        _scrollView->_fixedCellSize = 9.3f;
        _scrollView->_viewport = scrollRect;
        _scrollView->_contentRectTransform = ModifiersSection;
        container->Inject(_scrollView);
        //
        AddScrollbar();
        _scrollView->ManualAwake();
        SetupScrollbar();
    }

    void ModifiersCoreUIPatcher::AddScrollbar() {
        auto scrollbarPrefab = UnityEngine::Object::FindObjectsOfType<UnityEngine::RectTransform*>()->First([](auto x) { return x->name == StringW("ScrollBar"); });
        this->_scrollbarRect = UnityEngine::Object::Instantiate(scrollbarPrefab, Panel->transform, false);
        //setting up scroll view
        this->_scrollView->_verticalScrollIndicator = this->_scrollbarRect->Find("VerticalScrollIndicator")->GetComponent<HMUI::VerticalScrollIndicator*>();
        this->_scrollView->_pageUpButton = this->_scrollbarRect->Find("UpButton")->GetComponent<UnityEngine::UI::Button*>();
        this->_scrollView->_pageDownButton = this->_scrollbarRect->Find("DownButton")->GetComponent<UnityEngine::UI::Button*>();
    }

    void ModifiersCoreUIPatcher::SetupScrollbar() {
        this->_scrollbarRect->pivot = { 0.5, 0.5 };
        this->_scrollbarRect->sizeDelta = { 8, -24 };
        this->_scrollbarRect->localPosition = { 50, -39, 0 };
        this->_scrollView->SetContentSize(46);
    }
}