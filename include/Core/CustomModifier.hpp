#pragma once

#include "Modifier.hpp"
#include "UnityEngine/Color.hpp"

namespace ModifiersCoreQuest {
    struct CustomModifier : ModifiersCoreQuest::Modifier {
        UnityW<UnityEngine::Color> Color;
        UnityW<UnityEngine::Color> MultiplierColor;
    };
}