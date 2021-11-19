#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <vector>
#include <string>
#include <array>
#include <fstream>
#include "Screen.h"

class GameScreen : public Screen {
    private:
        /* Member Constants */
        static constexpr size_t spriteWidth {2};
        static constexpr Size<> mainSize {27, 82};
        static constexpr Size<> statBarSize {1, 26};
        static constexpr Size<> hotbarSize {10, 26};
        static constexpr Size<> consoleSize {12, 55};
        /* Member Enums */
        enum class SubWindowType {
            MAIN, STATBAR, HOTBAR, CONSOLE, COUNT
        };
        enum class ScreenFocus {
            MAIN, CONSOLE, OPTIONS
        };
        enum class CursorMove : int {
            LEFT = -1, RIGHT = 1
        };
        /* Member Structs */
        struct Player {
            size_t hp;
            size_t curHotbarSlot;
        };
        struct Console {
            /* If possible, use std::array over C-Style Array */
            static constexpr Size<> size[2] {{12, 55}, {0, 0}};
            enum class Mode {
                INTEGRATED, POPUP, COUNT
            } mode;
            struct Input {
                std::string line;
                size_t cursIndex; // Relative to line
                size_t cursPos; // Relative to console
                int highlight;
            } input;
            std::vector<std::pair<std::string, std::wstring>> record;
            std::ofstream file;
        };
        /* Member Variables */
        std::vector<std::unique_ptr<WINDOW, WindowDeleter>> subwins;
        std::array<std::string, 10> hotbar;
        Player p;
        ScreenFocus focus;
        Console cnsl;
        /* Private Member Methods */
        void consoleInput(int key);
        void drawStatBar();
        void hotbarSelect(size_t slot);
        void initScreen();
        void moveCursor(int side);
        void sendToConsole(std::string line, const std::wstring &icon); 
        void updateConsole();
        void updateConsoleLine(bool highlight=false);
    public:
        /* Inherit Constructor from Screen */
        GameScreen();
        void drawGraphics();
        void updateScreen();
        void userInput(int key);
};

#endif // GAMESCREEN_H