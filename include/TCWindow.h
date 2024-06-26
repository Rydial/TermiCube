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

namespace TC {

    /* Forward Declarations */
    struct WindowData;


    /* Type Alias */
    using WinData = WindowData;

}

typedef class TermiCubeWindow {
    private:
        /* Private Member Variables */
        std::shared_ptr<TC::WinData> data;
        /* Private Methods */
        void initCurses();
        void initColors();
        void initScreens();
    public:
        /* Public Constructor */
        TermiCubeWindow();
        /* Public Destructor */
        ~TermiCubeWindow();
        /* Public Methods */
        bool shouldClose();
        void update();
} TCWindow;


#endif // TCWINDOW_H
