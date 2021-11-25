#ifndef TCWINDOW_H
#define TCWINDOW_H


/*
Things to Remember:
    - Enums are ordered for a specific reason each

Things to do:
    - Replace deletor functor usage with pimpl-idioms
    - Reorganize entities with a sparse-set system (like EnTT)
    - Replace as MANY Magic Numbers as possible
*/

#include <memory>

/////////////////////// Forward Declarations ///////////////////////

struct TCWindowData;

/////////////////////// Game Window Class ///////////////////////

typedef class TermiCubeWindow {
    private:
        std::shared_ptr<TCWindowData> data;
        /* Private Methods */
        void initCurses();
        void initColors();
        void initScreens();
    public:
        TermiCubeWindow();
        ~TermiCubeWindow();
        bool shouldClose();
        void update();
} TCWindow;


#endif // TCWINDOW_H
