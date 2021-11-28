#ifndef SCREEN_H
#define SCREEN_H


#define _XOPEN_SOURCE_EXTENDED 1 /* Enables ncurses wide character support */

#include <panel.h>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>


namespace TC {

    /* Global Functions */
    size_t pyMod(int n, int mod);


    /* Forward Declarations */
    class WindowSharedData;


    /* Type Aliases */
    using Texts = std::unordered_map<std::string, std::pair<
        std::vector<std::string>, size_t>>;
    using WideChars = std::unordered_map<std::wstring, cchar_t>;
    using EntityChars = std::vector<std::string>;
    using EntChars = EntityChars;
    using WinSData = WindowSharedData;


    /* External Constants */
    extern const Texts texts;
    extern const WideChars boxCh;
    extern const EntChars entCh;

    /* Need a custom deleter function to use unique_ptr with an incomplete type
       Later on replace these with a pimpl-idiom */
    /* Custom Deleters */
    struct PanelDeleter {void operator()(PANEL *ptr) {del_panel(ptr);}};
    struct WindowDeleter {void operator()(WINDOW *ptr) {delwin(ptr);}};
    /* Custom Deleter Type Aliases */
    using PanelDel = PanelDeleter;
    using WinDel = WindowDeleter;


    /* Templates */
    template <typename T = int>
    struct Point {T y, x;};
    template <typename T = size_t>
    struct Size {T y, x;};


    struct Button {
        /* Public Member Variables */
        std::unique_ptr<WINDOW, WinDel> ptr;
        int yTop, yBtm, xLeft, xRight;
        std::function<void()> click;
        /* Maybe change to a general function that takes a button object
        and draws it */
        std::function<void(WINDOW *)> draw;
        /* Public Methods */
        Button(WINDOW *win, int y, int x, int yLen, int xLen,
            const std::function<void()> &click,
            const std::function<void(WINDOW *)> &draw);
        void highlight(int attrs);
    };
    /* Type Alias */
    using Btn = Button;


    class ButtonManager { 
        protected:
            /* Protected Methods */
            std::function<void(WINDOW *)> genDrawFunc(size_t maxLen,
                const std::string &txt, Size<> btnSize);
        public:
            /* Public Member Variables */
            std::vector<Button> list;
            size_t btn;
            /* Public Constructor */
            ButtonManager(WINDOW *win, Size<> size, Point<> startPos, int offset,
                std::vector<std::string> txts,
                std::function<std::function<void()>(size_t)> genClick);
            /* Public Virtual Destructor */
            virtual ~ButtonManager() = default;
    };
    /* Type Alias */
    using BtnMgr = ButtonManager;


    class Screen {
        protected:
            /* Protected Member Constants */
            static constexpr int maxRows {42}, maxCols {84};
            /* Protected Member Enums */
            enum class ScreenType {
                MAINMENU, GAME
            };
            /* Protected Member Structs */
            struct EventData {int key; MEVENT mouse;};
            struct Controls {
                int up, left, down, right;
                int enter;
            };
            /* Protected Static Member Variables */
            static EventData eData;
            static Controls control;
            /* Protected Member Variables */
            std::unique_ptr<WINDOW, WinDel> window;
            std::unique_ptr<PANEL, PanelDel> panel;
            /* Protected Virtual Methods */
            virtual void initScreen() = 0;
            /* Protected Methods */
            void drawBorder();
        public:
            /* Public Constructor */
            Screen();
            /* Public Virtual Destructor */
            virtual ~Screen() = default;
            /* Public Virtual Methods */
            virtual void drawGraphics() = 0;
            virtual void updateScreen() = 0;
            virtual void userInput(int key) = 0;
            /* Public Methods */
            PANEL * getPanel() {return panel.get();}
    };
    /* Type Alias */
    using Scr = Screen;
}


#endif
