#ifndef SCREEN_H
#define SCREEN_H

#define _XOPEN_SOURCE_EXTENDED 1 /* Enables ncurses wide character support */

#include <panel.h>
#include <memory>
#include <functional>
#include <unordered_map>

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






#endif
