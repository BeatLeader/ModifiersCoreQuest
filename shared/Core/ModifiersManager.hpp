#pragma once

#include "functional"
#include "CustomModifier.hpp"
#include "Modifier.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>

#include "GlobalNamespace/GameplayModifierParamsSO.hpp"
#include "GlobalNamespace/GameplayModifierToggle.hpp"

namespace ModifiersCoreQuest {

    class ModifiersManager {
        // Static class; Static Constructor is called in load()
        ModifiersManager() = delete;
        friend class ModifierPanelSpawner;
        friend class ModifiersCorePanel;
        friend class GameplayModifiersPanelPatch;

        public:
            // getters
            inline static auto get_Modifiers(){
                return std::views::values(AllModifiers);
            }
            inline static auto get_PendingModifiers(){
                return std::views::values(InternalPendingModifiers);
            }
            inline static auto get_CustomModifiers(){
                return std::views::values(InternalCustomModifiers);
            }
            inline static std::unordered_map<std::string, UnityW<GlobalNamespace::GameplayModifierParamsSO>> GameplayModifierParams = {};
            static std::optional<std::function<void(CustomModifier)>> ModifierAddedEvent;
            static std::optional<std::function<void(CustomModifier)>> ModifierRemovedEvent;

            // API
            static std::optional<Modifier> GetModifierWithId(std::string id);
            static bool HasModifierWithId(std::string id);
            static bool GetModifierState(std::string id);
            static void SetModifierState(std::string id, bool state);
            static void AddModifier(CustomModifier modifier);
            static void RemoveModifier(std::string id);
            static std::vector<std::tuple<SafePtrUnity<GlobalNamespace::GameplayModifierToggle>, Modifier>> Toggles();

            // Internal function, not private so its reachable in late_load
            static void LoadBaseGameModifiers();
        private:
            static std::map<std::string, ModifiersCoreQuest::CustomModifier> InternalCustomModifiers;
            static std::unordered_map<std::string, ModifiersCoreQuest::CustomModifier> InternalPendingModifiers;
            static std::unordered_map<std::string, ModifiersCoreQuest::Modifier> AllModifiers;

            static std::unordered_map<std::string, std::unordered_set<std::string>> DependentModifiers;
            static std::unordered_map<std::string, std::unordered_set<std::string>> ExclusiveModifiers;
            static std::unordered_map<std::string, std::unordered_set<std::string>> ExclusiveCategories;
            static std::unordered_map<std::string, std::unordered_set<std::string>> CategorizedModifiers;

            static std::vector<std::string> buffer;

            // API
            static void ReviewPendingModifiers();
            static void AddCustomModifierInternal(CustomModifier modifier, bool checkDependencies);
            static void AddModifierInternal(ModifiersCoreQuest::Modifier modifier);
            // Cache
            static void RemoveFromCache(std::string id, std::unordered_map<std::string, std::unordered_set<std::string>> dict);
            static void AddToCache(std::string id, std::optional<std::vector<std::string>> collection, std::unordered_map<std::string, std::unordered_set<std::string>>& dict, bool addSelf = false);
            static bool EnsureDependenciesExist(ModifiersCoreQuest::Modifier modifier);

            // Base Game (c++ doesnt have partial classes ._.)
            static std::optional<std::vector<std::string>> GetCategories(std::string id);
            static std::optional<std::vector<std::string>> GetExclusiveCategories(std::string id);
            static std::vector<std::string> MakeIdsArray(ArrayW<UnityW<GlobalNamespace::GameplayModifierParamsSO>> modifiers);
    };
}