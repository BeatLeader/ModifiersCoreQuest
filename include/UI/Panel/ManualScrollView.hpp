#pragma once

#include "custom-types/shared/macros.hpp"

#include "HMUI/ScrollView.hpp"

DECLARE_CLASS_CODEGEN(ModifiersCoreQuest, ManualScrollView, HMUI::ScrollView,
    DECLARE_INSTANCE_METHOD(void, Start);
    public:
        inline static bool _suppress = true;
        void ManualAwake();
)