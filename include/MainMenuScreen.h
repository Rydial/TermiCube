#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H

#include <string>
#include "TCWindow.h"
#include "Screen.h" 

class MainMenuScreen : public Screen {
    private:
        /* Member Constants */
        static constexpr int titlePosY {5};
        static constexpr Point<> btnSize {5, 50};
        static constexpr Point<> btnStartPos {14, (maxCols - btnSize.x) / 2};
        /* Member Enums */
        enum class ButtonType {
            NEWGAME, LOADGAME, SETTINGS, EXIT, COUNT
        };
        /* Member Structs */
        class ButtonManager {
            private:
                std::function<void()> genClickFunction(
                    TCWindowSharedData &winSData, int index);
                std::function<void(WINDOW *)> genDrawFunction(
                    const std::string &txt, size_t maxLen);
            public:
                std::vector<Button> list;
                size_t btn;
                /* Public Methods */
                ButtonManager(WINDOW *win, int startY, int startX,
                        std::shared_ptr<TCWindowData> &winData);
        };
        /* Member Variables */
        ButtonManager buttons;
        /* Private Member Methods */
        void initScreen();
    public:
        MainMenuScreen(std::shared_ptr<TCWindowData> &winData);
        void drawGraphics();
        void updateScreen();
        void userInput(int key);
};

#endif // MAINMENUSCREEN_H