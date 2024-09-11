#pragma once

#include "custom-types/shared/macros.hpp"

#include "HMUI/ScrollView.hpp"

DECLARE_CLASS_CODEGEN(ModifiersCoreQuest, ManualScrollView, HMUI::ScrollView,
    DECLARE_DEFAULT_CTOR();
    DECLARE_SIMPLE_DTOR(); 
    public:
        static bool _suppress;
        void ManualAwake();
)