#pragma once

#include "Core/CustomModifier.hpp"
#include "Core/Modifier.hpp"
#include "ModifierPanelBase.hpp"
#include "UI/Modifier/CustomModifierVisualsController.hpp"
#include "custom-types/shared/macros.hpp"

namespace ModifiersCoreQuest {
    class CustomModifierPanel : public ModifierPanelBase {

        public:
            CustomModifierPanel(UnityW<UnityEngine::GameObject> gameObjectParam) : ModifierPanelBase(gameObjectParam) {
                this->Awake();
            }
            Modifier get_Modifier();
            void SetModifier(CustomModifier customModifier);
        protected:
            void HandleToggleStateChanged(bool state);
            void Awake();
        private:
            CustomModifier _modifier;
            SafePtrUnity<ModifiersCoreQuest::CustomModifierVisualsController> _visualsController;
    };
}