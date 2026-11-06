#include "UI/Panel/ModifiersCoreUIPatcher.hpp"

#include "UnityEngine/Object.hpp"
#include "Zenject/Context.hpp"
#include "HMUI/Touchable.hpp"
#include "UnityEngine/UI/RectMask2D.hpp"
#include "UI/Panel/ScrollContentSizeUpdater.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/UI/GridLayoutGroup.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/TextAnchor.hpp"
#include "Zenject/DiContainer.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/Vector2.hpp"
#include "HMUI/VerticalScrollIndicator.hpp"
#include "System/Collections/Generic/List_1.hpp"
#include "logger.hpp"

#include <cmath>

DEFINE_TYPE(ModifiersCoreQuest, ModifiersCoreUIPatcher);

namespace ModifiersCoreQuest {
    static UnityEngine::RectTransform* CreateStretchedRect(StringW name, UnityEngine::Transform* parent) {
        auto go = UnityEngine::GameObject::New_ctor(name);
        go->layer = parent->gameObject->layer;
        auto rect = go->AddComponent<UnityEngine::RectTransform*>();
        rect->SetParent(parent, false);
        rect->anchorMin = UnityEngine::Vector2::get_zero();
        rect->anchorMax = UnityEngine::Vector2::get_one();
        rect->sizeDelta = UnityEngine::Vector2::get_zero();
        return rect;
    }

    void ModifiersCoreUIPatcher::Awake() {
        this->Panel = this->GetComponent<GlobalNamespace::GameplayModifiersPanelController*>();
        this->ModifiersSection = this->transform->Find("Modifiers").cast<UnityEngine::RectTransform>();
        this->PatchUI();
    }

    void ModifiersCoreUIPatcher::PatchUI() {
        auto contexts = UnityEngine::Object::FindObjectsOfType<Zenject::Context*>();
        auto context = contexts[contexts.size() - 1];
        auto container = context->Container;
        auto parent = this->GetComponentInParent(i2c::cs_type_of<GlobalNamespace::GameplaySetupViewController*>(), true);
        auto setupViewController = parent ? reinterpret_cast<GlobalNamespace::GameplaySetupViewController*>(parent.unsafe_ptr()) : nullptr;
        if (!setupViewController || !setupViewController->_selectionSegmentedControl) {
            getLogger().warn("No gameplay setup view around the modifiers panel, custom modifiers go without a tab");
            this->CustomModifiersSection = this->ModifiersSection;
            return;
        }
        this->CreateTabSelector(setupViewController->_selectionSegmentedControl, container);
        this->CreateCustomSection(setupViewController, container);
    }

    void ModifiersCoreUIPatcher::CreateTabSelector(HMUI::TextSegmentedControl* original, Zenject::DiContainer* container) {
        for (auto name : { "Info", "HintText" }) {
            if (auto child = this->transform->Find(name)) {
                auto rect = child.cast<UnityEngine::RectTransform>();
                rect->anchoredPosition = { rect->anchoredPosition.x, rect->anchoredPosition.y - 1.5f };
            }
        }
        this->TabSelector = UnityEngine::Object::Instantiate(original, this->transform, false);
        this->TabSelector->gameObject->name = "ModifiersCoreTabSelector";
        auto rect = this->TabSelector->transform.cast<UnityEngine::RectTransform>();
        for (int i = rect->childCount - 1; i >= 0; i--) {
            UnityEngine::Object::DestroyImmediate(rect->GetChild(i)->gameObject);
        }
        container->Inject(this->TabSelector);
        rect->anchorMin = { 0.5f, 1 };
        rect->anchorMax = { 0.5f, 1 };
        rect->pivot = { 0.5f, 1 };
        rect->sizeDelta = { 60, 5 };
        rect->anchoredPosition = { 0, -0.5f };
        this->TabSelector->_fontSize = 3.5f;
        auto texts = System::Collections::Generic::List_1<StringW>::New_ctor();
        texts->Add("Vanilla");
        texts->Add("Custom");
        this->TabSelector->SetTexts(texts->i___System__Collections__Generic__IReadOnlyList_1_T_(), nullptr);
    }

    void ModifiersCoreUIPatcher::CreateCustomSection(GlobalNamespace::GameplaySetupViewController* setupViewController, Zenject::DiContainer* container) {
        this->CustomModifiersContainer = CreateStretchedRect("CustomModifiers", this->transform);
        this->CustomModifiersContainer->anchorMin = this->ModifiersSection->anchorMin;
        this->CustomModifiersContainer->anchorMax = this->ModifiersSection->anchorMax;
        this->CustomModifiersContainer->pivot = this->ModifiersSection->pivot;
        this->CustomModifiersContainer->anchoredPosition = this->ModifiersSection->anchoredPosition;
        this->CustomModifiersContainer->sizeDelta = this->ModifiersSection->sizeDelta;
        //creating scroll view
        auto viewport = CreateStretchedRect("Viewport", this->CustomModifiersContainer);
        viewport->gameObject->AddComponent<HMUI::Touchable*>();
        viewport->gameObject->AddComponent<UnityEngine::UI::RectMask2D*>();
        this->_scrollView = viewport->gameObject->AddComponent<ModifiersCoreQuest::ManualScrollView*>();
        //setting up modifiers section
        this->CustomModifiersSection = CreateStretchedRect("Content", viewport);
        this->CustomModifiersSection->anchorMin = { 0, 1 };
        this->CustomModifiersSection->pivot = { 0.5f, 1 };
        auto vanillaGrid = this->ModifiersSection->GetComponent<UnityEngine::UI::GridLayoutGroup*>();
        auto cellSize = vanillaGrid->cellSize;
        auto spacing = vanillaGrid->spacing;
        float width = this->ModifiersSection->rect.width;
        int columns = std::max(1, (int)std::floor((width + spacing.x) / (cellSize.x + spacing.x)));
        int padding = (int)std::round((width - columns * cellSize.x - (columns - 1) * spacing.x) / 2);
        auto grid = this->CustomModifiersSection->gameObject->AddComponent<UnityEngine::UI::GridLayoutGroup*>();
        grid->cellSize = cellSize;
        grid->spacing = spacing;
        grid->startCorner = vanillaGrid->startCorner;
        grid->startAxis = vanillaGrid->startAxis;
        // Padded and left-aligned rather than centered, so a row that isn't full stays in the vanilla columns
        grid->padding = UnityEngine::RectOffset::New_ctor(padding, padding, 0, 0);
        grid->childAlignment = UnityEngine::TextAnchor::UpperLeft;
        grid->constraint = UnityEngine::UI::GridLayoutGroup::Constraint::FixedColumnCount;
        grid->constraintCount = columns;
        auto sizeFitter = this->CustomModifiersSection->gameObject->AddComponent<UnityEngine::UI::ContentSizeFitter*>();
        sizeFitter->verticalFit = UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize;
        //setting up scroll view
        this->_scrollView->_scrollViewDirection = HMUI::ScrollView::ScrollViewDirection::Vertical;
        this->_scrollView->_scrollType = HMUI::ScrollView::ScrollType::FixedCellSize;
        this->_scrollView->_fixedCellSize = cellSize.y + spacing.y;
        this->_scrollView->_viewport = viewport;
        this->_scrollView->_contentRectTransform = this->CustomModifiersSection;
        container->Inject(this->_scrollView);
        //
        this->AddScrollbar(setupViewController);
        this->_scrollView->ManualAwake();
        auto contentUpdater = this->CustomModifiersSection->gameObject->AddComponent<ModifiersCoreQuest::ScrollContentSizeUpdater*>();
        contentUpdater->scrollView = this->_scrollView;
        contentUpdater->scrollbar = this->_scrollbarRect->gameObject;
        contentUpdater->Refresh();
    }

    void ModifiersCoreUIPatcher::AddScrollbar(GlobalNamespace::GameplaySetupViewController* setupViewController) {
        auto findScrollbar = [](ArrayW<UnityEngine::RectTransform*> rects) -> UnityEngine::RectTransform* {
            for (auto rect : rects) {
                if (rect->name == StringW("ScrollBar")) return rect;
            }
            return nullptr;
        };
        auto scrollbarPrefab = findScrollbar(setupViewController->GetComponentsInChildren<UnityEngine::RectTransform*>(true));
        if (!scrollbarPrefab) {
            scrollbarPrefab = findScrollbar(UnityEngine::Object::FindObjectsOfType<UnityEngine::RectTransform*>());
        }
        this->_scrollbarRect = UnityEngine::Object::Instantiate(scrollbarPrefab, this->CustomModifiersContainer, false);
        this->_scrollbarRect->gameObject->SetActive(true);
        this->_scrollbarRect->anchorMin = { 1, 0 };
        this->_scrollbarRect->anchorMax = UnityEngine::Vector2::get_one();
        this->_scrollbarRect->pivot = { 0.5f, 0.5f };
        this->_scrollbarRect->sizeDelta = { 8, 0 };
        this->_scrollbarRect->anchoredPosition = UnityEngine::Vector2::get_zero();
        //setting up scroll view
        this->_scrollView->_verticalScrollIndicator = this->_scrollbarRect->Find("VerticalScrollIndicator")->GetComponent<HMUI::VerticalScrollIndicator*>();
        this->_scrollView->_pageUpButton = this->_scrollbarRect->Find("UpButton")->GetComponent<UnityEngine::UI::Button*>();
        this->_scrollView->_pageDownButton = this->_scrollbarRect->Find("DownButton")->GetComponent<UnityEngine::UI::Button*>();
    }
}