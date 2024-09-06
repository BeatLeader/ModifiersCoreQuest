#include "Core/CustomModifier.hpp"
#include "Core/Modifier.hpp"
#include "UI/Modifier/ModifierPanelBase.hpp"
#include "custom-types/shared/macros.hpp"

#include "UnityEngine/MonoBehaviour.hpp"
#include "UI/Panel/ModifiersCoreUIPatcher.hpp"
#include "UI/Modifier/ModifierPanelSpawner.hpp"
#include "UnityEngine/UI/GridLayoutGroup.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

DECLARE_CLASS_CODEGEN(ModifiersCoreQuest, ModifiersCorePanel, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_PRIVATE(ModifiersCoreQuest::ModifiersCoreUIPatcher*, _patcher);
    DECLARE_INSTANCE_FIELD_PRIVATE(ModifiersCoreQuest::ModifierPanelSpawner*, _spawner);
    DECLARE_INSTANCE_FIELD_PRIVATE(UnityEngine::UI::GridLayoutGroup*, _modifiersSectionGroup);

    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Start);
    DECLARE_INSTANCE_METHOD(void, OnDestroy);

    public:
        void SetModifierActive(Modifier modifier, bool state);
    private:
        UnityW<GlobalNamespace::GameplayModifiersPanelController> get_Panel();
        void RefreshModifiersOrder();
        void SetModifiersActive(auto modifiers, bool state, std::optional<std::string> except = {});
        void SetCategoriesActive(std::optional<std::vector<std::string>> categories, std::unordered_map<std::string, std::unordered_set<std::string>> cache, bool state, std::optional<std::string> except = {});
        void HandleModifierSpawnedInternal(ModifiersCoreQuest::ModifierPanelBase& panel);
        void HandleModifierAddedInternal(CustomModifier modifier);
        void HandleModifierAdded(CustomModifier modifier);
        void HandleModifierRemoved(CustomModifier modifier);
        void HandleModifierStateChanged(ModifiersCoreQuest::ModifierPanelBase& panel, bool state);

)