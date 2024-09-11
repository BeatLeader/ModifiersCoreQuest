#pragma once

#include "Core/Modifier.hpp"
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/GameObject.hpp"
#include "GlobalNamespace/GameplayModifierToggle.hpp"
#include "logger.hpp"
#include <functional>
#include <optional>

namespace ModifiersCoreQuest {
    class ModifierPanelBase {

        public:
            virtual Modifier get_Modifier() = 0;
            std::optional<std::function<void(ModifiersCoreQuest::ModifierPanelBase&, bool)>> ModifierStateChangedEvent;
            SafePtrUnity<UnityEngine::GameObject> gameObject;
            SafePtrUnity<GlobalNamespace::GameplayModifierToggle> _modifierToggle;
            void SetModifierActive(bool active);
        protected:
            ModifierPanelBase(UnityW<UnityEngine::GameObject> gameObjectParam) {
                this->gameObject=gameObjectParam;
            }
            virtual void Awake();
            virtual void HandleToggleStateChanged(bool state);
        private:
            SafePtrUnity<UnityEngine::UI::Toggle> _toggle;
    };
}