#ifndef TCWINDOW_H
#define TCWINDOW_H

#include <vector>
#include <memory>

/*
Things to Remember:
    - Enums are ordered for a specific reason each

Things to do:
    - Replace deletor functor usage with pimpl-idioms
    - Reorganize entities with a sparse-set system (like EnTT)
*/

/////////////////////// Forward Declarations ///////////////////////

class Screen;

/////////////////////// Transfer Data Wrappers ///////////////////////

struct TCWindowData {
    size_t screen;
    std::vector<std::unique_ptr<Screen>> screenList;
};

class TCWindowSharedData {
    private:
        /* Weak Ptr instead of Shared Ptr to prevent self-referencing */
        std::weak_ptr<TCWindowData> data;
    public:
        TCWindowSharedData(std::shared_ptr<TCWindowData> &winData);
        void switchScreen(size_t index);
};

/////////////////////// Game Window ///////////////////////

typedef class TermiCubeWindow {
    private:
        std::shared_ptr<TCWindowData> data;
        /* Private Methods */
        void initCurses();
        void initColors();
        void initWideChars();
        void initScreens();
        void terminate();
    public:
        TermiCubeWindow();
        int update();
} TCWindow;


#endif // TCWINDOW_H
