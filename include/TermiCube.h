#ifndef TERMICUBE_H
#define TERMICUBE_H

#include <vector>
#include <memory>
#include <iostream>
#include <panel.h>

/* Things to do:
    - Replace deletor functor usage with pimpl-idioms
    - Reorganize entities with a sparse-set system (like EnTT)
*/

/////////////////////// Screens ///////////////////////

class Screen {
    protected:
        struct Coordinate {int y, x;};
        struct DisplayItem {int y, x, yLen, xLen;};
        struct Button {int yTop, yBtm, xLeft, xRight;};
        /* Need a custom deleter function to use unique_ptr with an incomplete type */
        /* Later on replace these with a pimpl-idiom */
        struct PanelDeleter {void operator()(PANEL *ptr);};
        struct WindowDeleter {void operator()(WINDOW *ptr);};
        struct EventData {
            int key;
            MEVENT mouse;
        } static eData;
        static constexpr int maxRows {40}, maxCols {80};
        std::unique_ptr<WINDOW, WindowDeleter> window;
        std::unique_ptr<PANEL, PanelDeleter> panel;
    public:
        Screen();
        virtual ~Screen() = default;
        virtual void drawGraphics() = 0;
        virtual void updateScreen() = 0;
        virtual void userInput(int key) = 0;
};

class MainMenuScreen : public Screen {
    private:
        static constexpr Coordinate titleSize {6, 72};
        static constexpr Coordinate btnSize {5, 50};
        static constexpr DisplayItem title {6, 72, 5, (maxCols - titleSize.x) / 2};
        // static constexpr Button newGame {
        //     14 + 0, 14 + 0 + btnSize.y, }
        
        static constexpr Coordinate newGameBtnPos {14 + 0, (maxCols - btnSize.x) / 2};
        static constexpr Coordinate loadGameBtnPos {14 + 6, (maxCols - btnSize.x) / 2};
        static constexpr Coordinate settingsBtnPos {14 + 12, (maxCols - btnSize.x) / 2};
        static constexpr Coordinate creditsBtnPos {14 + 18, (maxCols - btnSize.x) / 2};
        std::unique_ptr<WINDOW, WindowDeleter> newGameBtn;
        std::unique_ptr<WINDOW, WindowDeleter> loadGameBtn;
        std::unique_ptr<WINDOW, WindowDeleter> settingsBtn;
        std::unique_ptr<WINDOW, WindowDeleter> creditsBtn;
    public:
        MainMenuScreen();
        void drawGraphics();
        void updateScreen();
        void userInput(int key);
};

class GameScreen : public Screen {
    public:
        /* Inherit Constructor from Screen */
        using Screen::Screen;
        void drawGraphics();
        void updateScreen();
        void userInput(int key);
};

///////////////////////////////////////////////////////

class GameWindow {
    private:
        enum class ScreenType {
            /* Order must follow initScreens() emplace_back order */
            MAINMENU
        };
        std::vector<std::unique_ptr<Screen>> screenList;
        ScreenType screen;
        bool exit;
        /* Private Methods */
        void initCurses();
        void initScreens();
        void terminate();
    public:
        GameWindow();
        int update();
};


#endif // TERMICUBE_H
