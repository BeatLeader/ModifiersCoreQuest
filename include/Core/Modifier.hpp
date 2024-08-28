#pragma once

#include <string>
#include <vector>
#include "UnityEngine/Sprite.hpp"

namespace ModifiersCoreQuest {
    struct Modifier{
        std::string Id;
        std::string Name;
        std::string Description;
        UnityW<UnityEngine::Sprite> Icon;
        float Multiplier;
        std::optional<std::vector<std::string>> Categories;
        std::optional<std::vector<std::string>> MutuallyExclusiveCategories;
        std::optional<std::vector<std::string>> MutuallyExclusiveModifiers;
        std::optional<std::vector<std::string>> RequiresModifiers;
        std::optional<std::vector<std::string>> RequiredByModifiers;
    };
}