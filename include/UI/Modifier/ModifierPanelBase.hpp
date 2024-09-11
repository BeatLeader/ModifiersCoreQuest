#pragma once

#include "Core/Modifier.hpp"
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/GameObject.hpp"
#include "GlobalNamespace/GameplayModifierToggle.hpp"
#include <functional>
#include <optional>

namespace ModifiersCoreQuest {
    class ModifierPanelBase {

        public:
            ModifierPanelBase(UnityW<UnityEngine::GameObject> gameObjectParam) {
                this->gameObject=gameObjectParam;
                this->Awake();
            }

            virtual Modifier get_Modifier() = 0;
            std::optional<std::function<void(ModifiersCoreQuest::ModifierPanelBase&, bool)>> ModifierStateChangedEvent;
            SafePtrUnity<UnityEngine::GameObject> gameObject;
            SafePtrUnity<GlobalNamespace::GameplayModifierToggle> get_modifierToggle();
            void SetModifierActive(bool active);
        protected:
            virtual void Awake();
            virtual void HandleToggleStateChanged(bool state);
        private:
            SafePtrUnity<UnityEngine::UI::Toggle> _toggle;
    };
}