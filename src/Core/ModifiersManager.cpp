#include "include/Core/ModifiersManager.hpp"

#include "config.hpp"
#include <ranges>
#include <stdexcept>
#include <string>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "include/Utils/ModifierUtils.hpp"
#include "UnityEngine/Resources.hpp"
#include "Patches/PlayerDataModelPatch.hpp"
#include "GlobalNamespace/PlayerData.hpp"
#include "logger.hpp"

namespace ModifiersCoreQuest {
    
    std::optional<std::function<void(CustomModifier)>> ModifiersManager::ModifierAddedEvent;
    std::optional<std::function<void(CustomModifier)>> ModifiersManager::ModifierRemovedEvent;

    // Public API
    std::optional<ModifiersCoreQuest::Modifier> ModifiersManager::GetModifierWithId(std::string id) {
        auto findIterator = AllModifiers.find(id);
        return findIterator == AllModifiers.end() ? std::optional<ModifiersCoreQuest::Modifier>() : findIterator->second;
    }

    bool ModifiersManager::HasModifierWithId(std::string id) {
        return AllModifiers.contains(id);
    }

    bool ModifiersManager::GetModifierState(std::string id) {
        if(!HasModifierWithId(id)){
            throw std::invalid_argument("A modifier with such identifier "+id+" does not exist");
        }
        // if custom
        if (InternalCustomModifiers.contains(id)){
            return getModConfig().ModifierStates.GetValue().contains(id);
        }
        // if base game
        auto playerData = PlayerDataModelPatch::get_PlayerData();
        if(!playerData.has_value()) {
            throw std::domain_error("Cannot get a base game modifier state until player data is loaded");
        }
        return ModifierUtils::GetGameplayModifierState(playerData.value()->gameplayModifiers, id);
    }

    void ModifiersManager::SetModifierState(std::string id, bool state) {
        if(!HasModifierWithId(id)){
            throw std::invalid_argument("A modifier with such identifier "+id+" does not exist");
        }
        auto map = getModConfig().ModifierStates.GetValue();
        map.insert_or_assign(id, state);
        getModConfig().ModifierStates.SetValue(map);
    }

    void ModifiersManager::AddModifier(CustomModifier modifier){
        AddCustomModifierInternal(modifier, true);
        ReviewPendingModifiers();
    }

    void ModifiersManager::RemoveModifier(std::string id){
        auto iter = InternalCustomModifiers.find(id);
        if (iter == InternalCustomModifiers.end()) {
            throw std::invalid_argument("A modifier with such key "+id+" does not exist");
        }
        auto modifier = iter->second;
        InternalCustomModifiers.erase(id);
        //removing from categories cache
        RemoveFromCache(modifier.Id, CategorizedModifiers);
        RemoveFromCache(modifier.Id, ExclusiveCategories);
        //removing from dependencies cache
        RemoveFromCache(modifier.Id, DependentModifiers);
        RemoveFromCache(modifier.Id, ExclusiveModifiers);
        //
        if(ModifierRemovedEvent.has_value()){
            ModifierRemovedEvent.value()(modifier);
        }
    }

    void ModifiersManager::AddCustomModifierInternal(CustomModifier modifier, bool checkDependencies){
        if (HasModifierWithId(modifier.Id)){
            throw std::invalid_argument("A modifier with the same key is already added");
        }
        if (modifier.Id.length() < 2 || modifier.Id.length() > 3) {
            throw std::invalid_argument("A modifier key should be 2 or 3 characters long (example: NF)");
        }
        if (checkDependencies && !EnsureDependenciesExist(modifier)) {
            ModifiersManager::InternalPendingModifiers.insert_or_assign(modifier.Id, modifier);
            return;
        }
        ModifiersManager::InternalCustomModifiers.insert_or_assign(modifier.Id, modifier);
        AddModifierInternal(modifier);
        if(ModifierAddedEvent.has_value()){
            ModifierAddedEvent.value()(modifier);
        }
    }

    void ModifiersManager::AddModifierInternal(ModifiersCoreQuest::Modifier modifier){
        ModifiersManager::AllModifiers.insert_or_assign(modifier.Id, modifier);
        //caching categories
        AddToCache(modifier.Id, modifier.Categories, CategorizedModifiers);
        AddToCache(modifier.Id, modifier.MutuallyExclusiveCategories, ExclusiveCategories);
        //caching dependencies
        AddToCache(modifier.Id, modifier.RequiresModifiers, DependentModifiers);
        AddToCache(modifier.Id, modifier.MutuallyExclusiveModifiers, ExclusiveModifiers, true);
    }

    void ModifiersManager::ReviewPendingModifiers() { 
        for (auto modifier : get_PendingModifiers()) {
          if (!EnsureDependenciesExist(modifier)) {
            continue;
          }
          AddCustomModifierInternal(modifier, false);
          buffer.push_back(modifier.Id);
        }
        for(auto id : buffer){
            InternalPendingModifiers.erase(id);
        }
        buffer.clear();
    }

    // Cache
    void ModifiersManager::RemoveFromCache(std::string id, std::unordered_map<std::string, std::unordered_set<std::string>> dict) {
        for(auto [_, set] : dict){
            set.erase(id);
        }
    }

    void ModifiersManager::AddToCache(std::string id, std::optional<std::vector<std::string>> collection, std::unordered_map<std::string, std::unordered_set<std::string>>& dict, bool addSelf){
        if(!collection.has_value()){
            return;
        }
        if(addSelf){
            auto& set = dict.emplace(id, std::unordered_set<std::string>()).first->second;
            for(auto item : collection.value()){
                set.emplace(item);
            }
        }
        for(auto item : collection.value()){
            auto& set = dict.emplace(item, std::unordered_set<std::string>()).first->second;
            set.emplace(id);
        }
    }

    bool ModifiersManager::EnsureDependenciesExist(ModifiersCoreQuest::Modifier modifier){
        if(!modifier.RequiresModifiers.has_value()){
            return true;
        }
        for(auto require : modifier.RequiredByModifiers.value()){
            if(!AllModifiers.contains(require)){
                return false;
            }
        }
        return true;
    }

    // Base Game (c++ doesnt have partial classes ._.)
    std::optional<std::vector<std::string>> ModifiersManager::GetCategories(std::string id){
        auto category = ModifierUtils::GetBaseModifierCategory(id);
        return category.has_value() ? (std::vector<std::string>) { category.value() } : std::optional<std::vector<std::string>>();
    }
    
    std::optional<std::vector<std::string>> ModifiersManager::GetExclusiveCategories(std::string id){
        auto category = ModifierUtils::GetBaseModifierExclusiveCategory(id);
        return category.has_value() ? (std::vector<std::string>) { category.value() } : std::optional<std::vector<std::string>>();
    }

    void ModifiersManager::LoadBaseGameModifiers(){
        auto modifiers = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::GameplayModifierParamsSO*>();
        for(auto modifier : modifiers){
            auto id = ModifierUtils::GetBaseModifierIdBySerializedName(modifier->modifierNameLocalizationKey);
            Modifier mod = {
                id,
                modifier->modifierNameLocalizationKey,
                modifier->descriptionLocalizationKey,
                (UnityEngine::Sprite*)modifier->icon,
                modifier->multiplier,
                GetCategories(id),
                GetExclusiveCategories(id),
                MakeIdsArray(modifier->mutuallyExclusives),
                MakeIdsArray(modifier->get_requires()),
                MakeIdsArray(modifier->get_requiredBy())
            };
            ModifiersManager::GameplayModifierParams.insert_or_assign(id, modifier);
            AddModifierInternal(mod);
        }
    }

    std::vector<std::string> ModifiersManager::MakeIdsArray(ArrayW<UnityW<GlobalNamespace::GameplayModifierParamsSO>> modifiers){
        std::vector<std::string> ret = {};
        for(UnityW<GlobalNamespace::GameplayModifierParamsSO> x : modifiers){
            ret.push_back(ModifierUtils::GetBaseModifierIdBySerializedName(x->modifierNameLocalizationKey));
        }
        return ret;
    }
}