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
    - Replace as MANY Magic Numbers as possible
*/

/////////////////////// Forward Declarations ///////////////////////

class Screen;

/////////////////////// Transfer Data Wrappers ///////////////////////

struct TCWindowData {
    bool exit;
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
        void terminate();
};

/////////////////////// Game Window ///////////////////////

typedef class TermiCubeWindow {
    private:
        std::shared_ptr<TCWindowData> data;
        /* Private Methods */
        void initCurses();
        void initColors();
        void initExtra();
        void initScreens();
    public:
        TermiCubeWindow();
        ~TermiCubeWindow();
        bool shouldClose();
        void update();
} TCWindow;


#endif // TCWINDOW_H
