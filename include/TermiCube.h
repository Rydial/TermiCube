#ifndef TERMICUBE_H
#define TERMICUBE_H

// #define _XOPEN_SOURCE_EXTENDED

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
        static constexpr int rows {40}, cols {80};
        /* Need a custom deleter function to use unique_ptr with an incomplete type */
        /* Later on replace these with a pimpl-idiom */
        struct Coordinate {int y, x;};
        struct PanelDeleter {void operator()(PANEL *ptr);};
        struct WindowDeleter {void operator()(WINDOW *ptr);};
        struct SubWindowDeleter {void operator()(WINDOW *ptr);};
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
        static constexpr Coordinate titlePos {5, (cols - titleSize.x) / 2};
        static constexpr Coordinate btnSize {5, 50};
        static constexpr Coordinate btnCreatePos {14, (cols - btnSize.x) / 2};
        std::unique_ptr<WINDOW, SubWindowDeleter> newGameBtn;
        std::unique_ptr<WINDOW, SubWindowDeleter> loadGameBtn;
        std::unique_ptr<WINDOW, SubWindowDeleter> settingsBtn;
        std::unique_ptr<WINDOW, SubWindowDeleter> creditsBtn;
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
