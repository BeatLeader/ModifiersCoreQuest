#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include "../Core/ModifierCategories.hpp"

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
            static bool GetGameplayModifierState(UnityW<GlobalNamespace::GameplayModifiers> modifiers, std::string id);
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

            inline static std::unordered_map<std::string, std::function<bool(GlobalNamespace::GameplayModifiers*)>> modifierGetters = {
                { "IF", [](GlobalNamespace::GameplayModifiers* x) { return x->instaFail; } },
                { "BE", [](GlobalNamespace::GameplayModifiers* x) { return x->energyType == GlobalNamespace::GameplayModifiers::EnergyType::Battery; } },
                { "NB", [](GlobalNamespace::GameplayModifiers* x) { return x->noBombs; } },
                { "NO", [](GlobalNamespace::GameplayModifiers* x) { return x->enabledObstacleType == GlobalNamespace::GameplayModifiers::EnabledObstacleType::NoObstacles; } },
                { "NA", [](GlobalNamespace::GameplayModifiers* x) { return x->noArrows; } },
                { "GN", [](GlobalNamespace::GameplayModifiers* x) { return x->ghostNotes; } },
                { "DA", [](GlobalNamespace::GameplayModifiers* x) { return x->disappearingArrows; } },
                { "SC", [](GlobalNamespace::GameplayModifiers* x) { return x->smallCubes; } },
                { "PM", [](GlobalNamespace::GameplayModifiers* x) { return x->proMode; } },
                { "SA", [](GlobalNamespace::GameplayModifiers* x) { return x->strictAngles; } },
                { "SS", [](GlobalNamespace::GameplayModifiers* x) { return x->songSpeed == GlobalNamespace::GameplayModifiers::SongSpeed::Slower; } },
                { "FS", [](GlobalNamespace::GameplayModifiers* x) { return x->songSpeed == GlobalNamespace::GameplayModifiers::SongSpeed::Faster; } },
                { "SF", [](GlobalNamespace::GameplayModifiers* x) { return x->songSpeed == GlobalNamespace::GameplayModifiers::SongSpeed::SuperFast; } },
                { "ZM", [](GlobalNamespace::GameplayModifiers* x) { return x->zenMode; } },
                { "NF", [](GlobalNamespace::GameplayModifiers* x) { return x->noFailOn0Energy; } },
            };
            
            static std::optional<std::string> GetBaseModifierCategory(std::string id);
            static std::optional<std::string> GetBaseModifierExclusiveCategory(std::string id);
    };
}