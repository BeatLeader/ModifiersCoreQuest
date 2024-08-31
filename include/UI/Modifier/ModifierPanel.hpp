#include "Core/Modifier.hpp"
#include "UI/Modifier/ModifierPanelBase.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CUSTOM(ModifiersCoreQuest, ModifierPanel, ModifierPanelBase,
    public:
        Modifier get_Modifier();
        void SetModifier(Modifier modifier);
    private:
        Modifier _modifier;
)