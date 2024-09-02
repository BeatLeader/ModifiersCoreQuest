#include "UI/Panel/ScrollContentSizeUpdater.hpp"

DEFINE_TYPE(ModifiersCoreQuest, ScrollContentSizeUpdater);

namespace ModifiersCoreQuest {
    void ScrollContentSizeUpdater::OnRectTransformDimensionsChange(){
        this->scrollView->UpdateContentSize();
    }
}