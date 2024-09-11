#include <optional>
#include <stdexcept>
#include <string>
#include "Utils/ModifierUtils.hpp"
#include "logger.hpp"

namespace ModifiersCoreQuest {

    std::unordered_map<std::string, std::string> ModifierUtils::modifierCategories = {                
        { "IF", ModifierCategories::Energy },
        { "BE", ModifierCategories::Energy },
        { "NB", ModifierCategories::Beatmap },
        { "NO", ModifierCategories::Beatmap },
        { "NA", ModifierCategories::Beatmap },
        { "GN", ModifierCategories::Visuals },
        { "DA", ModifierCategories::Visuals },
        { "SC", ModifierCategories::Beatmap },
        { "PM", ModifierCategories::Scoring },
        { "SA", ModifierCategories::Scoring },
        { "SS", ModifierCategories::Speed },
        { "FS", ModifierCategories::Speed },
        { "SF", ModifierCategories::Speed }
    };

    std::unordered_map<std::string, std::string> ModifierUtils::modifierExclusiveCategory = {                
        { "IF", ModifierCategories::Energy },
        { "BE", ModifierCategories::Energy },
        { "GN", ModifierCategories::Visuals },
        { "DA", ModifierCategories::Visuals },
        { "SS", ModifierCategories::Speed },
        { "FS", ModifierCategories::Speed },
        { "SF", ModifierCategories::Speed }
    };

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