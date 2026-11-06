#pragma once

// beatsaber-hook is a modding framework that lets us call functions and fetch field values from in the game
// It also allows creating objects, configuration, and importantly, hooking methods to modify their values
#include "paper2_scotland2/shared/logger.hpp"
#include "beatsaber-hook/shared/api.hpp"
#include "beatsaber-hook/shared/hooking.hpp"

// Includes for the functionality of the mod
#include "GlobalNamespace/GameplayCoreInstaller.hpp"
#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/PracticeSettings.hpp"
#include "GlobalNamespace/GameplayModifiersPanelController.hpp"
#include "GlobalNamespace/SettingsApplicatorSO.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/LevelCollectionViewController.hpp"
#include "GlobalNamespace/BeatmapLevel.hpp"
#include "GlobalNamespace/SongPreviewPlayer.hpp"
#include "GlobalNamespace/GameplaySetupViewController.hpp"
#include "System/Action.hpp"
#include "UnityEngine/AudioClip.hpp"
#include "HMUI/ViewController.hpp"
#include "HMUI/Touchable.hpp"

// Include the modloader header, which allows us to tell the modloader which mod this is, and the version etc.
#include "scotland2/shared/loader.hpp"

#include "include/logger.hpp"
#include "include/config.hpp"
#include "include/hooking.hpp"

void DidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);

void SetPitchSpeed(GlobalNamespace::GameplayModifiersPanelController* self);