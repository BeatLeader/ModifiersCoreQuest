#include "main.hpp"

#include "include/Core//ModifiersManager.hpp"

// Called at the early stages of game loading
extern "C" __attribute__((visibility("default"))) void setup(CModInfo* info)
{
    info->version = VERSION;
    info->id = MOD_ID;
    info->version_long = 0;
    modInfo.assign(*info);

    getModConfig().Init(modInfo);
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" __attribute__((visibility("default"))) void late_load()
{
    // Init things
    getModConfig().Init(modInfo);

    // Install Hooks
    auto logger = Paper::ConstLoggerContext("ModifiersCoreQuest");
    getLogger().info("Installing hooks...");
    // INSTALL_HOOK(logger, GameplayCoreInstallerInstallBindingsHook);
    getLogger().info("Installed all hooks!");

    ModifiersCoreQuest::ModifiersManager::LoadBaseGameModifiers();
}