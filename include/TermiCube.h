#ifndef TERMICUBE_H
#define TERMICUBE_H


#include <vector>
#include <memory>
#include <panel.h>


enum class ScreenType {
    /* Must follow order of emplace_back */
    GAME, MAINMENU
};


/////////////////////// Screens ///////////////////////

class Screen {
    protected:
        /* Need a custom deleter function to use unique_ptr with an incomplete type */
        /* Later on replace this with a pimpl-idiom */
        struct PanelDeleter {
            void operator()(PANEL *ptr) {del_panel(ptr);}
        };
        std::unique_ptr<PANEL, PanelDeleter> panel;
        static constexpr int rows {35}, cols {70};
    public:
        /* all four 0 means full screen */
        Screen();
        virtual ~Screen() = default;
        virtual void drawGraphics() = 0;
        virtual void updateScreen() = 0;
        virtual void userInput() = 0;
};

class MainMenuScreen : public Screen {
    public:
        /* Inherit Constructor from Screen */
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
        /* Private Methods */
        void initCurses();
        void initScreens();
    public:
        GameWindow();
        bool gameEnded() const {return exit;}
        void terminate() {exit = true;}
        void update();
};


#endif // TERMICUBE_H
