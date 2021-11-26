#include "TransferrableData.h"
#include "Screen.h"

////////////////////////////* Window Shared Data Class *///////////////////////////

TC::WinSData::WindowSharedData(std::shared_ptr<WinData> &gwData) :
    data{gwData}
{

}

/*==============================================================================*/

void TC::WinSData::switchScreen(size_t index)
{
    auto gwData {data.lock()};
    /* Checks if object is still available */
    if (gwData) {
        PANEL *ptr {};
        /* Hide all panels associated with the current screen */
        while ((ptr = panel_below(0)) != NULL)
            hide_panel(ptr);
        /* Switch screen */
        gwData->screen = index;
        show_panel(gwData->screenList[gwData->screen]->getPanel());
        update_panels();
        doupdate();
    }
}

void TC::WinSData::terminate()
{
    auto gwData {data.lock()};
    /* Checks if object is still available */
    if (gwData)
        gwData->exit = true;
}