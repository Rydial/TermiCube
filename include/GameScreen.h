#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <vector>
#include <string>
#include <array>
#include "Screen.h"

class GameScreen : public Screen {
    private:
        /* Member Constants */
        static constexpr size_t spriteWidth {2};
        static constexpr Coordinate mainSize {27, 82};
        static constexpr Coordinate statBarSize {1, 26};
        static constexpr Coordinate hotbarSize {10, 26};
        static constexpr Coordinate consoleSize {12, 55};
        /* Member Enums */
        enum class SubWindowType {
            MAIN, STATBAR, HOTBAR, CONSOLE, COUNT
        };
        enum class ScreenFocus {
            MAIN, CONSOLE, OPTIONS
        };
        /* Member Structs */
        struct Player {
            size_t hp;
            size_t curHotbarSlot;
        };
        struct Console {
            int cursorXPos;
            std::string curLine;
            std::vector<std::string> record;
            // std::ifstream test;
        };
        /* Member Variables */
        std::vector<std::unique_ptr<WINDOW, WindowDeleter>> subwins;
        std::array<std::string, 10> hotbar;
        Player p;
        ScreenFocus focus;
        Console console;
        /* Private Member Methods */
        void initScreen();
        void drawStatBar();
        void hotbarSelect(size_t slot);
        void consoleInput(int key);
    public:
        /* Inherit Constructor from Screen */
        GameScreen();
        void drawGraphics();
        void updateScreen();
        void userInput(int key);
};

#endif // GAMESCREEN_H