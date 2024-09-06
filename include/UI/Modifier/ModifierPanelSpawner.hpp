#include "Core/CustomModifier.hpp"
#include "ModifierPanelBase.hpp"
#include "custom-types/shared/macros.hpp"

#include "UnityEngine/MonoBehaviour.hpp"
#include "UI/Panel/ModifiersCoreUIPatcher.hpp"
#include "UI/Modifier/CustomModifierPanel.hpp"
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>

DECLARE_CLASS_CODEGEN(ModifiersCoreQuest, ModifierPanelSpawner, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_PRIVATE(ModifiersCoreQuest::ModifiersCoreUIPatcher*, _patcher);
    DECLARE_INSTANCE_FIELD_PRIVATE(UnityEngine::GameObject*, _modifierPrefab);

    DECLARE_INSTANCE_METHOD(void, Awake);

    public:
        inline auto get_Panels() {
            return std::views::values(this->_spawnedPanels);
        }
        ModifiersCoreQuest::ModifierPanelBase& GetSpawnedPanel(std::string id);
        ModifiersCoreQuest::ModifierPanelBase* SpawnPanel(CustomModifier modifier);
        void DespawnPanel(std::string id);
    private:
        std::unordered_map<std::string, ModifiersCoreQuest::ModifierPanelBase*> _spawnedPanels = {};
        std::unordered_map<std::string, ModifiersCoreQuest::ModifierPanelBase*> _baseGamePanels = {};
        std::stack<ModifiersCoreQuest::CustomModifierPanel*> _pooledPanels = {};
        UnityW<UnityEngine::RectTransform> get_ModifiersSection();
        void LoadExistingModifiers();
)