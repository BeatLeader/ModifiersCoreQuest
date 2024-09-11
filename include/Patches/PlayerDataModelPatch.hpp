#include "GlobalNamespace/PlayerDataModel.hpp"
#include <optional>

namespace ModifiersCoreQuest {

    class PlayerDataModelPatch {
        // static class
        PlayerDataModelPatch() = delete;
        public:
            static std::optional<GlobalNamespace::PlayerData*> get_PlayerData();
            static std::optional<UnityW<GlobalNamespace::PlayerDataModel>> _playerDataModel;
    };
}