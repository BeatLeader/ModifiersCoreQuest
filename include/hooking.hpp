#pragma once

#include "beatsaber-hook/shared/hooking.hpp"
#include <vector>

namespace ModifiersCoreQuest {
    class Hooking {
    private:
        inline static std::vector<void (*)()> installFuncs;

    public:
        static void AddInstallFunc(void (*installFunc)()) {
            installFuncs.push_back(installFunc);
        }

        static inline void InstallHooks() {
            for (auto& func : installFuncs) func();
        }
    };
}

#define HOOK_AUTO_INSTALL_ORIG(name_)                                                               \
    struct Auto_Hook_##name_ {                                                                      \
        static void Auto_Hook_##name_##_Install() {                                                 \
            static constexpr auto logger = Paper::ConstLoggerContext(MOD_ID "_Install_" #name_);    \
            ::i2c::install_hook_orig<hook_##name_>(logger);                                         \
        }                                                                                           \
        Auto_Hook_##name_() { ::ModifiersCoreQuest::Hooking::AddInstallFunc(Auto_Hook_##name_##_Install); }   \
    };                                                                                              \
    static Auto_Hook_##name_ Auto_Hook_Instance_##name_

#define HOOK_AUTO_INSTALL(name_)                                                                    \
    struct Auto_Hook_##name_ {                                                                      \
        static void Auto_Hook_##name_##_Install() {                                                 \
            static constexpr auto logger = Paper::ConstLoggerContext(MOD_ID "_Install_" #name_);    \
            ::i2c::install_hook<hook_##name_>(logger);                                              \
        }                                                                                           \
        Auto_Hook_##name_() { ::ModifiersCoreQuest::Hooking::AddInstallFunc(Auto_Hook_##name_##_Install); }   \
    };                                                                                              \
    static Auto_Hook_##name_ Auto_Hook_Instance_##name_

#define __MCQ_HOOK_MATCH_STRUCT(name_, method, ret_type, ...)                                                                    \
    struct BS_HOOK_HIDDEN hook_##name_ {                                                                                         \
        static constexpr auto cast_test = []<typename T>() { return requires { static_cast<T>(method); }; };                     \
        using func_t = ret_type (*)(__VA_ARGS__);                                                                                \
        using cast_t = ::i2c::detail::method_check<cast_test, func_t>::type;                                                    \
        static_assert(cast_test.operator()<cast_t>(), "Hook method signature does not match!");                                  \
        static_assert(::i2c::detail::match_hookable<static_cast<cast_t>(method)>, "Method cannot be hooked!");                  \
        __INTERNAL_HOOK_STRUCT(name_, ::i2c::metadata_getter<static_cast<cast_t>(method)>::method_info(), ret_type, __VA_ARGS__) \
    }

#define MAKE_AUTO_HOOK_MATCH(name_, mPtr, retval, ...)            \
    __MCQ_HOOK_MATCH_STRUCT(name_, mPtr, retval, __VA_ARGS__);    \
    HOOK_AUTO_INSTALL(name_);                                     \
    retval hook_##name_::hook_m_##name_(__VA_ARGS__)

#define MAKE_AUTO_HOOK_ORIG_MATCH(name_, mPtr, retval, ...)       \
    __MCQ_HOOK_MATCH_STRUCT(name_, mPtr, retval, __VA_ARGS__);    \
    HOOK_AUTO_INSTALL_ORIG(name_);                                \
    retval hook_##name_::hook_m_##name_(__VA_ARGS__)
