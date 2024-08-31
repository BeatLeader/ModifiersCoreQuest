#pragma once

#include "Modifier.hpp"
#include "UnityEngine/Color.hpp"

namespace ModifiersCoreQuest {
    struct CustomModifier : ModifiersCoreQuest::Modifier {
        std::optional<UnityEngine::Color> Color;
        std::optional<UnityEngine::Color> MultiplierColor;
    };
}