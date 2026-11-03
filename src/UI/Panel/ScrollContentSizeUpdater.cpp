#include "UI/Panel/ScrollContentSizeUpdater.hpp"

#include "UnityEngine/Rect.hpp"
#include "UnityEngine/RectTransform.hpp"

DEFINE_TYPE(ModifiersCoreQuest, ScrollContentSizeUpdater);

namespace ModifiersCoreQuest {
    void ScrollContentSizeUpdater::Refresh() {
        this->scrollView->UpdateContentSize();
        this->scrollbar->SetActive(this->scrollView->_contentRectTransform->rect.height > this->scrollView->_viewport->rect.height + 0.01f);
    }

    void ScrollContentSizeUpdater::OnRectTransformDimensionsChange(){
        this->Refresh();
    }
}