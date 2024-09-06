#include "UI/Panel/ManualScrollView.hpp"
#include "UI/Panel/ModifiersCoreUIPatcher.hpp"
#include "UnityEngine/Resources.hpp"
#include "hooking.hpp"
#include "logger.hpp"

DEFINE_TYPE(ModifiersCoreQuest, ManualScrollView);

namespace ModifiersCoreQuest {

    void ManualScrollView::ManualAwake(){
        ManualScrollView::_suppress = false;
        this->Awake();
        ManualScrollView::_suppress = true;
    }

    MAKE_AUTO_HOOK_ORIG_MATCH(ScrollViewAwake, &HMUI::ScrollView::Awake, void, HMUI::ScrollView* self){
        if(!il2cpp_utils::try_cast<ModifiersCoreQuest::ManualScrollView>(self) || !ManualScrollView::_suppress){
            ScrollViewAwake(self);
        }
    }
}