#ifndef __TERMICUBE_H__
#define __TERMICUBE_H__


#include <vector>
#include <memory>
#include <curses.h>


enum class ScreenType {
    MAINMENU, /*SETTINGS,*/ GAME, /*INVENTORY*/
};


/////////////////////// Screens ///////////////////////

class Screen {
    private:
        /* Need a custom deleter function to use unique_ptr with an incomplete type */
        struct WINDOWDeleter {
            void operator()(WINDOW *ptr) {delwin(ptr);}
        };
        std::unique_ptr<WINDOW, WINDOWDeleter> window;
    public:
        Screen(int row=0, int col=0, int y=0, int x=0);
        virtual ~Screen() = default;
        virtual void drawGraphics() = 0;
        virtual void updateScreen() = 0;
        virtual void userInput() = 0;
};

class MainMenuScreen : public Screen {
    public:
        using Screen::Screen;
        void drawGraphics();
        void updateScreen();
        void userInput();
};

class GameScreen : public Screen {
    public:
        using Screen::Screen;
        void drawGraphics();
        void updateScreen();
        void userInput();
};

///////////////////////////////////////////////////////

class GameWindow {
    private:
        std::vector<std::unique_ptr<Screen>> screenList;
        ScreenType screen;
        bool exit;

        void initCurses();
        void initScreens();
    public:
        GameWindow() :
            screenList{},
            screen{ScreenType::MAINMENU},
            exit{false}
        {
            initCurses();
            initScreens();
        }
        bool gameEnded() const {return exit;}
        void pollEvents();
        void terminate() {exit = true;}
        void updateWindow();
};


#endif // __TERMICUBE_H__
