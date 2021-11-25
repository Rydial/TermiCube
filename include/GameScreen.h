#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <vector>
#include <string>
#include <array>
#include <fstream>
#include "Screen.h"

class GameScreen : public Screen {
    private:
        /* Private Member Constants */
        static constexpr size_t spriteWidth {2};
        static constexpr Size<> mainSize {27, 82};
        static constexpr Size<> statBarSize {1, 26};
        static constexpr Size<> hotbarSize {10, 26};
        static constexpr Size<> consoleSize {12, 55};
        /* Private Member Enums */
        enum class SubWindowType {
            MAIN, STATBAR, HOTBAR, CONSOLE, COUNT
        };
        enum class ScreenFocus {
            MAIN, OPTIONS, CONSOLE
        };
        enum class CursorMove : int {
            LEFT = -1, RIGHT = 1
        };
        /* Private Member Structs */
        struct Player {
            size_t hp;
            size_t curHotbarSlot;
        };
        struct Console {
            /* Public Member Constants */
            static constexpr Size<> size[2] {{12, 55}, {0, 0}};
            /* Public Member Enums */
            enum class Mode {
                INTEGRATED, POPUP, COUNT
            } mode;
            /* Public Member Structs */
            struct Input {
                std::string line;
                size_t cursIndex; // Relative to line
                size_t cursPos; // Relative to console
                int highlight;
            } input;
            /* Public Member Variables */
            std::vector<std::pair<std::string, std::wstring>> record;
            std::ofstream file;
        };
        struct OptionMenu {
            static constexpr Size<> size {27, 82};
            std::unique_ptr<PANEL, PanelDeleter> panel;
            std::vector<std::unique_ptr<WINDOW, WindowDeleter>> subWins;
        };
        /* Private Member Variables */
        std::vector<std::unique_ptr<WINDOW, WindowDeleter>> subWins;
        std::array<std::string, 10> hotbar;
        Console cnsl;
        OptionMenu optMenu;
        Player p;
        ScreenFocus focus;
        /* Private Methods */
        void consoleInput(int key);
        void drawStatBar();
        void hotbarSelect(size_t slot);
        void initConsole();
        void initOptionMenu();
        void initScreen();
        void initSubWindows();
        void moveCursor(int side, bool highlight=false);
        void sendToConsole(std::string line, const std::wstring &icon); 
        void updateConsole();
        
    public:
        /* Public Constructor */
        GameScreen();
        /* Public Methods */
        void drawGraphics();
        void updateScreen();
        void userInput(int key);
};

#endif // GAMESCREEN_H