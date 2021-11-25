#include "Screen.h"
#include "TransferrableData.h"

TCWindowSharedData::TCWindowSharedData(std::shared_ptr<TCWindowData> &gwData) :
    data{gwData}
{

}

void TCWindowSharedData::switchScreen(size_t index)
{
    auto gwData {data.lock()};
    /* Checks if object is still available */
    if (gwData) {
        hide_panel(gwData->screenList[gwData->screen]->getPanel());
        gwData->screen = index;
        show_panel(gwData->screenList[gwData->screen]->getPanel());
        update_panels();
        doupdate();
    }
}

void TCWindowSharedData::terminate()
{
    auto gwData {data.lock()};
    /* Checks if object is still available */
    if (gwData)
        gwData->exit = true;
}