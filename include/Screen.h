#ifndef SCREEN_H
#define SCREEN_H


#define _XOPEN_SOURCE_EXTENDED 1 /* Enables ncurses wide character support */

#include <panel.h>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>


class Screen {
    private:
        /* Private Type Aliases */
        using Texts = std::unordered_map<std::string, std::pair<
            std::vector<std::string>, size_t>>;
        using WideChars = std::unordered_map<std::wstring, cchar_t>;
        /* Private Static Methods */
        static Texts genTexts();
        // static WideChars genWideChars();
    protected:
        /* Need a custom deleter function to use unique_ptr with an incomplete type
           Later on replace these with a pimpl-idiom */
        struct PanelDeleter {void operator()(PANEL *ptr) {del_panel(ptr);}};
        struct WindowDeleter {void operator()(WINDOW *ptr) {delwin(ptr);}};
        /* Protected Member Constants */
        static constexpr int maxRows {42}, maxCols {84};
        static const Texts texts;
        static WideChars wchars;
        /* Protected Member Enums */
        enum class ScreenType {
            /* Follow the order of emplace_back in screenList */ 
            MAINMENU, GAME
        };
        /* Protected Template Members */
        template <typename T = int>
        struct Point {T y, x;};
        template <typename T = size_t>
        struct Size {T y, x;};
        /* Protected Member Structs */
        struct EventData {int key; MEVENT mouse;};
        struct Controls {
            int up, left, down, right;
            int enter;
        };
        /* Protected Member Classes */
        class Button {
            public:
                /* Public Member Variables */
                std::unique_ptr<WINDOW, WindowDeleter> ptr;
                int yTop, yBtm, xLeft, xRight;
                std::function<void()> click;
                std::function<void(WINDOW *)> draw;
                /* Public Methods */
                Button(WINDOW *win, int y, int x, int yLen, int xLen,
                        std::function<void()> click,
                        std::function<void(WINDOW *)> draw);
                void highlight(int attrs);
        };
        // class ButtonManager {
        //     private:
        //         /* Private Member Variables */
        //         std::vector<Button> list;
        //         size_t btn;
        //         /* Private Member Functions */
        //         void initButtons(WINDOW *win, int y, int x,
        //             size_t yLen, size_t xLen, size_t n);
        //     public:
        //         /* Public Constructor */
        //         ButtonManager(WINDOW *win, int y, int x,
        //             size_t yLen, size_t xLen, size_t n);
        // };
        /* Protected Static Member Variables */
        static EventData eData;
        static Controls control;
        /* Protected Member Variables */
        std::unique_ptr<WINDOW, WindowDeleter> window;
        std::unique_ptr<PANEL, PanelDeleter> panel;
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


#endif
