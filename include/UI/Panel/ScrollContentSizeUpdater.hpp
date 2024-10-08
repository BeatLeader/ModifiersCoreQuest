#include "custom-types/shared/macros.hpp"

#include "UnityEngine/MonoBehaviour.hpp"
#include "HMUI/ScrollView.hpp"

DECLARE_CLASS_CODEGEN(ModifiersCoreQuest, ScrollContentSizeUpdater, UnityEngine::MonoBehaviour,
    DECLARE_DEFAULT_CTOR();
    DECLARE_SIMPLE_DTOR(); 
    DECLARE_INSTANCE_FIELD(HMUI::ScrollView*, scrollView);
    DECLARE_INSTANCE_METHOD(void, OnRectTransformDimensionsChange);
)