#include "Patches/PlayerDataModelPatch.hpp"
#include "hooking.hpp"
#include <optional>

namespace ModifiersCoreQuest {
    std::optional<UnityW<GlobalNamespace::PlayerDataModel>> PlayerDataModelPatch::_playerDataModel;

    std::optional<SafePtr<GlobalNamespace::PlayerData>> PlayerDataModelPatch::get_PlayerData() {
        return PlayerDataModelPatch::_playerDataModel.has_value() ? PlayerDataModelPatch::_playerDataModel.value()->playerData : std::optional<SafePtr<GlobalNamespace::PlayerData>>();
    }

    MAKE_AUTO_HOOK_MATCH(PlayerDataModelInjectHook, &GlobalNamespace::PlayerDataModel::Inject, void, GlobalNamespace::PlayerDataModel* self, StringW playerDataJsonString, GlobalNamespace::PlayerDataFileModel* playerDataFileModel) {
        PlayerDataModelInjectHook(self, playerDataJsonString, playerDataFileModel);
        PlayerDataModelPatch::_playerDataModel = self;
    }
}