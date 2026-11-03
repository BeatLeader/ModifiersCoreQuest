#pragma once

#include "config-utils/shared/config-utils.hpp"
#include <string>
#include <unordered_map>

DECLARE_CONFIG(ModConfig) {
    CONFIG_VALUE(ModifierStates, StringKeyedMap<bool>, "ModifierStates", StringKeyedMap<bool>());
    CONFIG_VALUE(SelectedTab, int, "SelectedTab", 0);
};
