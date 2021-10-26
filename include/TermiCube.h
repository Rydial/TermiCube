#ifndef TERMICUBE_H
#define TERMICUBE_H

#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <panel.h>

/*
Things to Remember:
    - Enums are ordered for a specific reason each

Things to do:
    - Replace deletor functor usage with pimpl-idioms
    - Reorganize entities with a sparse-set system (like EnTT)
*/


/////////////////////// Screens ///////////////////////

class Screen {
    protected:
        /* Member Constants */
        static constexpr int maxRows {40}, maxCols {80};
        /* Need a custom deleter function to use unique_ptr with an incomplete type */
        /* Later on replace these with a pimpl-idiom */
        struct PanelDeleter {void operator()(PANEL *ptr) {del_panel(ptr);}};
        struct WindowDeleter {void operator()(WINDOW *ptr) {delwin(ptr);}};
        /* Member Enums */
        enum class ScreenType {
            /* Follow the order of emplace_back in screenList */ 
            MAINMENU, GAME
        };
        /* Member Structs */
        struct Coordinate {int y, x;};
        struct DisplayItem {};
        struct Button {
            std::unique_ptr<WINDOW, WindowDeleter> ptr;
            int yTop, yBtm, xLeft, xRight;
            std::function<void()> click;
            std::function<void()> draw;
            /* Public Methods */
            Button(WINDOW *win, int y, int x, int yLen, int xLen,
                    std::function<void()> click, std::function<void()> draw);
            void highlight(int attrs);
        };
        struct EventData {int key; MEVENT mouse;} static eData;
        struct Controls {int up, left, down, right;} static control;
        /* Member Variables */
        std::unique_ptr<WINDOW, WindowDeleter> window;
        std::unique_ptr<PANEL, PanelDeleter> panel;
        /* Private Methods */
        virtual void initScreen() = 0;
    public:
        Screen(); /* Constructor */
        virtual ~Screen() = default; /* Virtual Destructor */
        /* Virtual Methods */
        virtual void drawGraphics() = 0;
        virtual void updateScreen() = 0;
        virtual void userInput(int key) = 0;
        /* Public Methods */
        PANEL * getPanel() {return panel.get();}
};

class MainMenuScreen : public Screen {
    private:
        /* Member Constants */
        static constexpr Coordinate titleSize {6, 72};
        static constexpr Coordinate btnSize {5, 50};
        static constexpr Coordinate titlePos {5, (maxCols - titleSize.x) / 2};
        static constexpr Coordinate btnStartPos {14, (maxCols - btnSize.x) / 2};
        /* Member Enums */
        enum class ButtonType {
            NEWGAME, LOADGAME, SETTINGS, CREDITS, COUNT
        };
        /* Member Structs */
        struct ButtonManager {
            private:
                std::function<void()> genClickFunction(WINDOW *win);
                std::function<void()> genDrawFunction(
                    WINDOW *win, std::vector<std::string> &txt);
            public:
                std::vector<Button> list;
                size_t btn;
                /* Public Methods */
                ButtonManager(WINDOW *win, int startY, int startX, size_t &curScreen);
        };
        /* Member Variables */
        ButtonManager buttons;
        /* Private Member Methods */
        void initScreen();
        static void parseTxt(std::vector<std::string> &txt, std::string path);
    public:
        MainMenuScreen(size_t &curScreen);
        void drawGraphics();
        void updateScreen();
        void userInput(int key);
};

// class NewGameScreen : public Screen {
//     private:
//         /* Private Member Methods */
//         void initScreen();
//     public:
//         /* Inherit Constructor from Screen */
//         using Screen::Screen;
//         void drawGraphics();
//         void updateScreen();
//         void userInput(int key);
// };

class GameScreen : public Screen {
    private:
        /* Private Member Methods */
        void initScreen();
    public:
        /* Inherit Constructor from Screen */
        using Screen::Screen;
        void drawGraphics();
        void updateScreen();
        void userInput(int key);
};

/////////////////////// Game Window ///////////////////////

class GameWindow {
    private:
        size_t screen;
        std::vector<std::unique_ptr<Screen>> screenList;
        /* Private Methods */
        void initCurses();
        void initScreens();
        void terminate();
    public:
        GameWindow();
        int update();
};


#endif // TERMICUBE_H
