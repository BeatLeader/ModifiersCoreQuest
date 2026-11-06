#pragma once

#include "beatsaber-hook/shared/safeptr.hpp"

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
            safe_ptr<UnityEngine::GameObject*> gameObject;
            safe_ptr<GlobalNamespace::GameplayModifierToggle*> _modifierToggle;
            void SetModifierActive(bool active);
        protected:
            ModifierPanelBase(UnityW<UnityEngine::GameObject> gameObjectParam) {
                this->gameObject=gameObjectParam;
            }
            virtual void Awake();
            virtual void HandleToggleStateChanged(bool state);
        private:
            safe_ptr<UnityEngine::UI::Toggle*> _toggle;
    };
}