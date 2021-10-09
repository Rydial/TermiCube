#ifndef TERMICUBE_H
#define TERMICUBE_H

#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <panel.h>

/* Things to do:
    - Replace deletor functor usage with pimpl-idioms
    - Reorganize entities with a sparse-set system (like EnTT)
*/


/////////////////////// Screens ///////////////////////

class Screen {
    protected:
        /* Need a custom deleter function to use unique_ptr with an incomplete type */
        /* Later on replace these with a pimpl-idiom */
        struct PanelDeleter {void operator()(PANEL *ptr);};
        struct WindowDeleter {void operator()(WINDOW *ptr);};
        /* Structs */
        struct Coordinate {int y, x;};
        struct DisplayItem {};
        struct Button {
            int yTop, yBtm, xLeft, xRight;
            std::unique_ptr<WINDOW, WindowDeleter> btn;
            std::function<void()> click;
            /* Public Methods */
            Button();
        };
        struct EventData {
            int key;
            MEVENT mouse;
        } static eData;
        struct Controls {
            int up, left, down, right;
        } static control;
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
        enum ButtonType {
            NEWGAME, LOADGAME, SETTINGS, CREDITS
        };
        class ButtonManager {
            private:
                Button newGame;
                Button loadGame;
                Button settings;
                Button credits;
                std::vector<Button &> temp;
                Button &current;
            public:
                WINDOW * get() {return current.btn.get();};
                void previous();
                void next();
            
        };
        static constexpr Coordinate titleSize {6, 72};
        static constexpr Coordinate btnSize {5, 50};
        static constexpr Coordinate titlePos {5, (maxCols - titleSize.x) / 2};

        Button newGame, loadGame, settings, credits;

        // static constexpr Button newGame {14 + 0, 14 + 0 + btnSize.y,
        //     (maxCols - btnSize.x) / 2, ((maxCols - btnSize.x) / 2) + btnSize.x};
        // static constexpr Button loadGame {14 + 6, 14 + 6 + btnSize.y,
        //     newGame.xLeft, newGame.xLeft + btnSize.x};
        // static constexpr Button settings {14 + 12, 14 + 12 + btnSize.y,
        //     newGame.xLeft, newGame.xLeft + btnSize.x};
        // static constexpr Button credits {14 + 18, 14 + 18 + btnSize.y,
        //     newGame.xLeft, newGame.xLeft + btnSize.x};
        // std::unique_ptr<WINDOW, WindowDeleter> newGameBtn;
        // std::unique_ptr<WINDOW, WindowDeleter> loadGameBtn;
        // std::unique_ptr<WINDOW, WindowDeleter> settingsBtn;
        // std::unique_ptr<WINDOW, WindowDeleter> creditsBtn;
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
