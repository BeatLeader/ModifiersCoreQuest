#include "UI/Panel/ModifiersCorePanel.hpp"

#include "Core/CustomModifier.hpp"
#include "Core/Modifier.hpp"
#include "UI/Modifier/ModifierPanelBase.hpp"
#include "UI/Panel/ModifiersCoreUIPatcher.hpp"
#include "Core/ModifiersManager.hpp"
#include "UnityEngine/GameObject.hpp"
#include <functional>
#include <unordered_set>
#include "custom-types/shared/delegate.hpp"
#include "GlobalNamespace/GameplayModifiersModelSO.hpp"
#include "System/Action.hpp"
#include "System/Action_2.hpp"
#include "HMUI/SegmentedControl.hpp"
#include "HMUI/TextSegmentedControl.hpp"
#include "config.hpp"
#include "logger.hpp"

DEFINE_TYPE(ModifiersCoreQuest, ModifiersCorePanel);

namespace ModifiersCoreQuest {
    UnityW<GlobalNamespace::GameplayModifiersPanelController> ModifiersCorePanel::get_Panel() {
        return this->_patcher->Panel;
    }

    void ModifiersCorePanel::Awake() {
        this->_patcher = this->gameObject->AddComponent<ModifiersCoreQuest::ModifiersCoreUIPatcher*>();
        this->_spawner = this->gameObject->AddComponent<ModifiersCoreQuest::ModifierPanelSpawner*>();
        if (this->_patcher->CustomModifiersSection == this->_patcher->ModifiersSection) {
            this->_modifiersSectionGroup = this->_patcher->ModifiersSection->GetComponent<UnityEngine::UI::GridLayoutGroup*>();
            this->_modifiersSectionGroup->enabled = true;
        }
    }

    void ModifiersCorePanel::Start() {
        //handling already spawned
        for(auto& panel : this->_spawner->get_Panels()){
            this->HandleModifierSpawnedInternal(*panel);
        }
        ModifiersCoreQuest::ModifiersManager::ModifierAddedEvent = (std::function<void(CustomModifier)>)[this](CustomModifier modifier) { this->HandleModifierAdded(modifier); };
        ModifiersCoreQuest::ModifiersManager::ModifierRemovedEvent = (std::function<void(CustomModifier)>)[this](CustomModifier modifier) { this->HandleModifierRemoved(modifier); };
        if (this->_patcher->TabSelector) {
            this->_patcher->TabSelector->add_didSelectCellEvent(custom_types::MakeDelegate<System::Action_2<UnityW<HMUI::SegmentedControl>, int32_t>*>(
                (std::function<void(UnityW<HMUI::SegmentedControl>, int32_t)>)[this](UnityW<HMUI::SegmentedControl>, int32_t cellIdx) { this->HandleTabSelected(cellIdx); }
            ));
        }
        for(auto modifier : ModifiersCoreQuest::ModifiersManager::get_CustomModifiers()){
            this->HandleModifierAddedInternal(modifier);
        }
        this->RefreshTabs();
    }

    void ModifiersCorePanel::RefreshTabs() {
        if (!this->_patcher->TabSelector) {
            return;
        }
        bool hasCustomModifiers = !ModifiersCoreQuest::ModifiersManager::get_CustomModifiers().empty();
        this->_patcher->TabSelector->gameObject->SetActive(hasCustomModifiers);
        this->ShowTab(hasCustomModifiers && getModConfig().SelectedTab.GetValue() == CustomTab ? CustomTab : VanillaTab);
    }

    void ModifiersCorePanel::ShowTab(int tab) {
        this->_patcher->TabSelector->SelectCellWithNumber(tab);
        this->_patcher->ModifiersSection->gameObject->SetActive(tab == VanillaTab);
        this->_patcher->CustomModifiersContainer->gameObject->SetActive(tab == CustomTab);
    }

    void ModifiersCorePanel::HandleTabSelected(int tab) {
        getModConfig().SelectedTab.SetValue(tab);
        this->ShowTab(tab);
    }

    void ModifiersCorePanel::OnDestroy() {
        ModifiersCoreQuest::ModifiersManager::ModifierAddedEvent = {};
        ModifiersCoreQuest::ModifiersManager::ModifierRemovedEvent = {};
    }

    void ModifiersCorePanel::RefreshModifiersOrder() {
        for (auto modifier : ModifiersCoreQuest::ModifiersManager::get_CustomModifiers()) {
            this->_spawner->GetSpawnedPanel(modifier.Id).gameObject->transform->SetAsLastSibling();
        }
    }

    void ModifiersCorePanel::SetModifiersActive(auto modifiers, bool state, std::optional<std::string> except) {
        if(modifiers.empty()){
            return;
        }
        for(auto modifier : modifiers){
            if(except.has_value() && modifier == except.value()) {
                continue;
            }
            if (auto panel = this->_spawner->TryGetSpawnedPanel(modifier)) {
                panel->SetModifierActive(state);
            }
        }
    }

    void ModifiersCorePanel::SetCategoriesActive(std::optional<std::vector<std::string>> categories, std::unordered_map<std::string, std::unordered_set<std::string>> cache, bool state, std::optional<std::string> except) {
        if(!categories.has_value()){
            return;
        }
        for(auto category : categories.value()){
            auto it = cache.find(category);
            auto modifiers = it != cache.end() ? it->second : std::unordered_set<std::string>();
            this->SetModifiersActive(modifiers, state, except);
        }
    }

    void ModifiersCorePanel::SetModifierActive(Modifier modifier, bool state){
        if(this->get_Panel()->_changingGameplayModifierToggles) {
            return;
        }
        this->get_Panel()->_changingGameplayModifierToggles = true;
        if(state) {
            //disabling categories that cannot be used with this modifier
            this->SetCategoriesActive(modifier.MutuallyExclusiveCategories, ModifiersManager::CategorizedModifiers, false, modifier.Id);
            //disabling modifiers that cannot be used with this category
            this->SetCategoriesActive(modifier.Categories, ModifiersManager::ExclusiveCategories, false, modifier.Id);
            //disabling modifiers that cannot be used with this one
            auto it = ModifiersManager::ExclusiveModifiers.find(modifier.Id);
            auto exclusiveModifiers = it != ModifiersManager::ExclusiveModifiers.end() ? it->second : std::unordered_set<std::string>();
            this->SetModifiersActive(exclusiveModifiers, false);
            //enabling modifiers that required for this one to work
            this->SetModifiersActive(modifier.RequiredByModifiers.value_or(std::vector<std::string>()), true);
        }
        else {
            auto it = ModifiersManager::DependentModifiers.find(modifier.Id);
            auto dependOnThis = it != ModifiersManager::DependentModifiers.end() ? it->second : std::unordered_set<std::string>();
            this->SetModifiersActive(dependOnThis, false);
        }
        this->get_Panel()->_gameplayModifiers = this->get_Panel()->_gameplayModifiersModel->CreateGameplayModifiers(custom_types::MakeDelegate<System::Func_2<UnityW<GlobalNamespace::GameplayModifierParamsSO>, bool>*>((std::function<bool(UnityW<GlobalNamespace::GameplayModifierParamsSO>)>)[this] (UnityW<GlobalNamespace::GameplayModifierParamsSO> params) {
            return this->get_Panel()->GetToggleValueWithGameplayModifierParams(params);
        }));
        this->get_Panel()->_changingGameplayModifierToggles = false;
        this->get_Panel()->RefreshTotalMultiplierAndRankUI();
        // The game raises this event only for its own toggles
        if (ModifiersManager::InternalCustomModifiers.contains(modifier.Id) && this->get_Panel()->didChangeGameplayModifiersEvent) {
            this->get_Panel()->didChangeGameplayModifiersEvent->Invoke();
        }
    }

    void ModifiersCorePanel::HandleModifierSpawnedInternal(ModifierPanelBase& panel) {
        panel.ModifierStateChangedEvent = (std::function<void(ModifierPanelBase&, bool)>)[this](ModifierPanelBase& panel, bool state) { return this->HandleModifierStateChanged(panel, state); };
    }

    void ModifiersCorePanel::HandleModifierAddedInternal(CustomModifier modifier){
        auto panel = this->_spawner->SpawnPanel(modifier);
        this->HandleModifierSpawnedInternal(*panel);
    }

    void ModifiersCorePanel::HandleModifierAdded(CustomModifier modifier){
        this->HandleModifierAddedInternal(modifier);
        this->RefreshModifiersOrder();
        this->RefreshTabs();
    }

    void ModifiersCorePanel::HandleModifierRemoved(CustomModifier modifier) {
        auto& panel = this->_spawner->GetSpawnedPanel(modifier.Id);
        panel.ModifierStateChangedEvent = {};
        this->_spawner->DespawnPanel(modifier.Id);
        this->RefreshModifiersOrder();
        this->RefreshTabs();
    }

    void ModifiersCorePanel::HandleModifierStateChanged(ModifierPanelBase& panel, bool state) {
        this->SetModifierActive(panel.get_Modifier(), state);
    }
}