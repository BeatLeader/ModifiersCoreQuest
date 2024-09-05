#include "main.hpp"

#include "include/Core/ModifiersManager.hpp"
#include "custom-types/shared/register.hpp"

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
    il2cpp_functions::Init();
    custom_types::Register::AutoRegister();
    ModifiersCoreQuest::Hooking::InstallHooks();
}