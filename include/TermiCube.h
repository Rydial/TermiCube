#ifndef TERMICUBE_H
#define TERMICUBE_H

/* Enables ncurses wide character support */
#define _XOPEN_SOURCE_EXTENDED 1

#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include <panel.h>

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

struct GameWindowData {
    size_t screen;
    std::vector<std::unique_ptr<Screen>> screenList;
};

class GameWindowSharedData {
    private:
        /* Weak Ptr instead of Shared Ptr to prevent self-referencing */
        std::weak_ptr<GameWindowData> data;
    public:
        GameWindowSharedData(std::shared_ptr<GameWindowData> &gwData);
        void switchScreen(size_t index);
};

/////////////////////// Screens ///////////////////////

class Screen {
    protected:
        /* Need a custom deleter function to use unique_ptr with an incomplete type
           Later on replace these with a pimpl-idiom */
        struct PanelDeleter {void operator()(PANEL *ptr) {del_panel(ptr);}};
        struct WindowDeleter {void operator()(WINDOW *ptr) {delwin(ptr);}};

        /* Member Constants */
        static constexpr int maxRows {42}, maxCols {84};
        /* Member Enums */
        enum class ScreenType {
            /* Follow the order of emplace_back in screenList */ 
            MAINMENU, GAME
        };
        /* Member Structs */
        struct Coordinate {int y, x;};
        struct DisplayItem {};
        struct EventData {int key; MEVENT mouse;};
        struct Controls {
            int up, left, down, right;
            int enter;
        };
        /* Member Classes */
        class Button {
            public:
                std::unique_ptr<WINDOW, WindowDeleter> ptr;
                int yTop, yBtm, xLeft, xRight;
                std::function<void()> click;
                std::function<void(WINDOW *)> draw;
                /* Public Methods */
                Button(WINDOW *win, int y, int x, int yLen, int xLen,
                        std::function<void()> click, std::function<void(WINDOW *)> draw);
                void highlight(int attrs);
        };
        /* Static Member Variables */
        static EventData eData;
        static Controls control;
        static std::unordered_map<std::wstring, cchar_t> wchars;
        /* Member Variables */
        std::unique_ptr<WINDOW, WindowDeleter> window;
        std::unique_ptr<PANEL, PanelDeleter> panel;
        /* Private Methods */
        virtual void initScreen() = 0;
        void drawBorder();
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
        static constexpr int titlePosY {5};
        static constexpr Coordinate btnSize {5, 50};
        static constexpr Coordinate btnStartPos {14, (maxCols - btnSize.x) / 2};
        /* Member Enums */
        enum class ButtonType {
            NEWGAME, LOADGAME, SETTINGS, CREDITS, COUNT
        };
        /* Member Structs */
        struct ButtonManager {
            private:
                std::function<void()> genClickFunction(
                    GameWindowSharedData &gwSData, int index);
                std::function<void(WINDOW *)> genDrawFunction(
                    std::vector<std::string> &txt, size_t maxLen);
            public:
                std::vector<Button> list;
                size_t btn;
                /* Public Methods */
                ButtonManager(WINDOW *win, int startY, int startX,
                        std::shared_ptr<GameWindowData> &gwData);
        };
        /* Member Variables */
        ButtonManager buttons;
        /* Private Member Methods */
        void initScreen();
        void initWideChars();
    public:
        MainMenuScreen(std::shared_ptr<GameWindowData> &gwData);
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
        /* Member Constants */
        static constexpr Coordinate mainSize {27, 82};
        static constexpr Coordinate hpBarSize {1, 30};
        /* Member Enums */
        enum class SubWindowType {
            MAIN, HPBAR, COUNT
        };
        /* Member Variables */
        std::vector<std::unique_ptr<WINDOW, WindowDeleter>> subwins;
        size_t hp {3};
        /* Private Member Methods */
        void initScreen();
        void drawHPBar();
    public:
        /* Inherit Constructor from Screen */
        GameScreen();
        void drawGraphics();
        void updateScreen();
        void userInput(int key);
};

/////////////////////// Game Window ///////////////////////

typedef class TermiCubeWindow {
    private:
        std::shared_ptr<GameWindowData> data;
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


#endif // TERMICUBE_H
