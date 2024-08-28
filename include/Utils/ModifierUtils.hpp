#include <functional>
#include <string>
#include <unordered_map>
#include "include/Core/ModifierCategories.hpp"

#include "GlobalNamespace/GameplayModifiers.hpp"
#include "UnityEngine/Object.hpp"

namespace ModifiersCoreQuest {
    class ModifierUtils{
        // Static class
        ModifierUtils() = delete;
        // Internals freigeschaltet für: (workaround für c# internal modifier)
        friend class ModifiersManager;

        public:
            static bool TryGetBaseModifierIdBySerializedName(std::string name, std::string& id);
            static std::string GetBaseModifierIdBySerializedName(std::string name);
            static bool IsValidBaseGameModifier(std::string id);
            static bool GetGameplayModifierState(SafePtr<GlobalNamespace::GameplayModifiers> modifiers, std::string id);
        private:
            inline static std::unordered_map<std::string, std::string> defaultModifierIds = {
                { "MODIFIER_ONE_LIFE", "IF" },
                { "MODIFIER_FOUR_LIVES", "BE" },
                { "MODIFIER_NO_BOMBS", "NB" },
                { "MODIFIER_NO_OBSTACLES", "NO" },
                { "MODIFIER_NO_ARROWS", "NA" },
                { "MODIFIER_GHOST_NOTES", "GN" },
                { "MODIFIER_DISAPPEARING_ARROWS", "DA" },
                { "MODIFIER_SMALL_CUBES", "SC" },
                { "MODIFIER_PRO_MODE", "PM" },
                { "MODIFIER_STRICT_ANGLES", "SA" },
                { "MODIFIER_SLOWER_SONG", "SS" },
                { "MODIFIER_FASTER_SONG", "FS" },
                { "MODIFIER_SUPER_FAST_NOTES", "SF" },
                { "MODIFIER_SUPER_FAST_SONG", "SF" },
                { "MODIFIER_ZEN_MODE", "ZM" },
                { "MODIFIER_NO_FAIL_ON_0_ENERGY", "NF" }
            };

            inline static std::unordered_map<std::string, std::string> modifierCategories = {
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

            inline static std::unordered_map<std::string, std::string> modifierExclusiveCategory = {
                { "IF", ModifierCategories::Energy },
                { "BE", ModifierCategories::Energy },
                { "GN", ModifierCategories::Visuals },
                { "DA", ModifierCategories::Visuals },
                { "SS", ModifierCategories::Speed },
                { "FS", ModifierCategories::Speed },
                { "SF", ModifierCategories::Speed }
            };

            inline static std::unordered_map<std::string, std::function<bool(SafePtr<GlobalNamespace::GameplayModifiers>)>> modifierGetters = {
                { "IF", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->instaFail; } },
                { "BE", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->energyType == GlobalNamespace::GameplayModifiers::EnergyType::Battery; } },
                { "NB", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->noBombs; } },
                { "NO", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->enabledObstacleType == GlobalNamespace::GameplayModifiers::EnabledObstacleType::NoObstacles; } },
                { "NA", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->noArrows; } },
                { "GN", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->ghostNotes; } },
                { "DA", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->disappearingArrows; } },
                { "SC", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->smallCubes; } },
                { "PM", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->proMode; } },
                { "SA", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->strictAngles; } },
                { "SS", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->songSpeed == GlobalNamespace::GameplayModifiers::SongSpeed::Slower; } },
                { "FS", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->songSpeed == GlobalNamespace::GameplayModifiers::SongSpeed::Faster; } },
                { "SF", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->songSpeed == GlobalNamespace::GameplayModifiers::SongSpeed::SuperFast; } },
                { "ZM", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->zenMode; } },
                { "NF", [](SafePtr<GlobalNamespace::GameplayModifiers> x) { return x->noFailOn0Energy; } },
            };
            
            static std::optional<std::string> GetBaseModifierCategory(std::string id);
            static std::optional<std::string> GetBaseModifierExclusiveCategory(std::string id);
    };
}