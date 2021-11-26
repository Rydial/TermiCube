#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H


#include <string>

namespace TC {

    class MainMenuScreen : public Screen {
        private:
            /* Private Member Constants */
            static constexpr int titlePosY {5};
            static constexpr Size<> btnSize {5, 50};
            static constexpr Point<> btnStartPos {14, (maxCols - btnSize.x) / 2};
            /* Private Static Methods */
            static std::function<void()> genClickFunc(WinSData &winSData, int index);
            /* Private Member Enums */
            enum class ButtonType {
                NEWGAME, LOADGAME, SETTINGS, EXIT, COUNT
            };
            /* Private Member Variables */
            BtnMgr btns;
            /* Private Methods */
            void initScreen();
        public:
            /* Public Constructor*/
            MainMenuScreen(WinSData &winSData);
            /* Public Methods */
            void drawGraphics();
            void updateScreen();
            void userInput(int key);
    };
    /* Type Alias */
    using MMScr = MainMenuScreen;

}


#endif // MAINMENUSCREEN_H