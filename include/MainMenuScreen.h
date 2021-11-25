#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H

#include <string>
#include "Screen.h" 

/////////////////////// Forward Declarations ///////////////////////

class TCWindowSharedData;

/////////////////////// MainMenu Screen Class ///////////////////////

class MainMenuScreen : public Screen {
    private:
        /* Private Member Constants */
        static constexpr int titlePosY {5};
        static constexpr Size<> btnSize {5, 50};
        static constexpr Point<> btnStartPos {14, (maxCols - btnSize.x) / 2};
        /* Private Member Enums */
        enum class ButtonType {
            NEWGAME, LOADGAME, SETTINGS, EXIT, COUNT
        };
        /* Private Member Structs */
        class MMSButtonManager : public ButtonManager {
            private:
                /* Private Methods */
                std::function<void()> genClickFunction(
                    TCWindowSharedData &winSData, int index);
            public:
                /* Public Constructor */
                MMSButtonManager(WINDOW *win,
                    TCWindowSharedData &winSData);
        };
        /* Private Member Variables */
        MMSButtonManager buttons;
        /* Private Member Methods */
        void initScreen();
    public:
        /* Public Constructor*/
        MainMenuScreen(TCWindowSharedData &winSData);
        /* Public Methods */
        void drawGraphics();
        void updateScreen();
        void userInput(int key);
};

#endif // MAINMENUSCREEN_H