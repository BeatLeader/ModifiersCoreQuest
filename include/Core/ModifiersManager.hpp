#include "functional"
#include "CustomModifier.hpp"
#include "Modifier.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "GlobalNamespace/GameplayModifierParamsSO.hpp"

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
            static std::optional<std::function<void(CustomModifier)>> ModifierAddedEvent;
            static std::optional<std::function<void(CustomModifier)>> ModifierRemovedEvent;

            // API
            static std::optional<Modifier> GetModifierWithId(std::string id);
            static bool HasModifierWithId(std::string id);
            static bool GetModifierState(std::string id);
            static void SetModifierState(std::string id, bool state);
            static void AddModifier(CustomModifier modifier);
            static void RemoveModifier(std::string id);

            // Internal function, not private so its reachable in late_load
            static void LoadBaseGameModifiers();
        private:
            inline static std::unordered_map<std::string, ModifiersCoreQuest::CustomModifier> InternalCustomModifiers = {};
            inline static std::unordered_map<std::string, ModifiersCoreQuest::CustomModifier> InternalPendingModifiers = {};
            inline static std::unordered_map<std::string, ModifiersCoreQuest::Modifier> AllModifiers = {};

            inline static std::unordered_map<std::string, std::unordered_set<std::string>> DependentModifiers = {};
            inline static std::unordered_map<std::string, std::unordered_set<std::string>> ExclusiveModifiers = {};
            inline static std::unordered_map<std::string, std::unordered_set<std::string>> ExclusiveCategories = {};
            inline static std::unordered_map<std::string, std::unordered_set<std::string>> CategorizedModifiers = {};

            inline static std::vector<std::string> buffer = {};

            // API
            static void ReviewPendingModifiers();
            static void AddCustomModifierInternal(CustomModifier modifier, bool checkDependencies);
            static void AddModifierInternal(ModifiersCoreQuest::Modifier modifier);
            // Cache
            static void RemoveFromCache(std::string id, std::unordered_map<std::string, std::unordered_set<std::string>> dict);
            static void AddToCache(std::string id, std::optional<std::vector<std::string>> collection, std::unordered_map<std::string, std::unordered_set<std::string>>& dict, bool addSelf = false);
            static bool EnsureDependenciesExist(ModifiersCoreQuest::Modifier modifier);

            // Base Game (c++ doesnt have partial classes ._.)
            inline static std::unordered_map<std::string, UnityW<GlobalNamespace::GameplayModifierParamsSO>> GameplayModifierParams = {};
            static std::optional<std::vector<std::string>> GetCategories(std::string id);
            static std::optional<std::vector<std::string>> GetExclusiveCategories(std::string id);
            static std::vector<std::string> MakeIdsArray(ArrayW<UnityW<GlobalNamespace::GameplayModifierParamsSO>> modifiers);
    };
}