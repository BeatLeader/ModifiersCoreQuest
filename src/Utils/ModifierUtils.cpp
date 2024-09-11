#include <optional>
#include <stdexcept>
#include <string>
#include "Utils/ModifierUtils.hpp"

namespace ModifiersCoreQuest {
    bool ModifierUtils::TryGetBaseModifierIdBySerializedName(std::string name, std::string& id){
        auto iter = defaultModifierIds.find(name);
        id = iter == defaultModifierIds.end() ? NULL : iter->second;
        return iter != defaultModifierIds.end();
    }

    std::string ModifierUtils::GetBaseModifierIdBySerializedName(std::string name){
        auto iter = defaultModifierIds.find(name);
        if(iter == defaultModifierIds.end()){
            throw std::invalid_argument("Unknown base game modifier name: " + name);
        }
        return iter->second;
    }

    bool ModifierUtils::IsValidBaseGameModifier(std::string id){
        return modifierGetters.contains(id);
    }

    bool ModifierUtils::GetGameplayModifierState(UnityW<GlobalNamespace::GameplayModifiers> modifiers, std::string id){
        auto iter = modifierGetters.find(id);
        if(iter == modifierGetters.end()){
            throw std::invalid_argument("Unknown base game modifier id: " + id);
        }

        return iter->second(modifiers);
    }

    std::optional<std::string> ModifierUtils::GetBaseModifierCategory(std::string id) {
        auto iter = modifierCategories.find(id);
        return iter == modifierCategories.end() ? std::optional<std::string>() : iter->second;
    }

    std::optional<std::string> ModifierUtils::GetBaseModifierExclusiveCategory(std::string id){
        auto iter = modifierExclusiveCategory.find(id);
        return iter == modifierExclusiveCategory.end() ? std::optional<std::string>() : iter->second;
    }
}