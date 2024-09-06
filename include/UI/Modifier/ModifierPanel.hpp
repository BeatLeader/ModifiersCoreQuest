#include "Core/Modifier.hpp"
#include "UI/Modifier/ModifierPanelBase.hpp"
#include "custom-types/shared/macros.hpp"

namespace ModifiersCoreQuest {
    class ModifierPanel : public ModifierPanelBase {
        using ModifierPanelBase::ModifierPanelBase;
        public:
            Modifier get_Modifier();
            void SetModifier(Modifier modifier);
        private:
            Modifier _modifier;
    };
}