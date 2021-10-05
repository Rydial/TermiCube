#ifndef __TERMICUBE_H__
#define __TERMICUBE_H__


#include <vector>
#include <memory>
#include <curses.h>


enum class ScreenType {
    MAINMENU, /*SETTINGS,*/ GAME, /*INVENTORY*/
};

/* Need a custom deleter function to use unique_ptr with an incomplete type */
struct WINDOWDeleter {
    void operator()(WINDOW *ptr) {delwin(ptr);}
};


/////////////////////// Screens ///////////////////////

class Screen {
    private:
        std::unique_ptr<WINDOW, WINDOWDeleter> window;
    public:
        Screen();
        virtual ~Screen() = default;
        virtual void drawGraphics() = 0;
        virtual void updateScreen() = 0;
        virtual void userInput() = 0;
};

class MainMenuScreen : public Screen {
    public:
        void drawGraphics();
        void updateScreen();
        void userInput();
};

class GameScreen : public Screen {
    public:
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
    public:
        GameWindow() :
            screenList{},
            screen{ScreenType::MAINMENU},
            exit{false}
        {
            initCurses();
            /* unique_ptr are not copyable, and initializer lists only use copy semantics
               so emplace_back had to be used instead */
            screenList.emplace_back(std::make_unique<MainMenuScreen>());
            screenList.emplace_back(std::make_unique<GameScreen>());
        }
        bool gameEnded() const {return exit;}
        void pollEvents();
        void terminate() {exit = true;}
        void updateWindow();
};


#endif // __TERMICUBE_H__
