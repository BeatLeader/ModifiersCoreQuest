#include "Core/Modifier.hpp"
#include "UI/Modifier/ModifierPanelBase.hpp"
#include "custom-types/shared/macros.hpp"

namespace ModifiersCoreQuest {
    class ModifierPanel : public ModifierPanelBase {
        public:
            ModifierPanel(UnityW<UnityEngine::GameObject> gameObjectParam) : ModifierPanelBase(gameObjectParam) {
                this->Awake();
            }
            Modifier get_Modifier();
            void SetModifier(Modifier modifier);
        private:
            Modifier _modifier;
    };
}