#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <vector>
#include <array>
#include <string>
#include "Screen.h"

class GameScreen : public Screen {
    private:
        /* Member Constants */
        static constexpr size_t spriteWidth {2};
        static constexpr Coordinate mainSize {27, 82};
        static constexpr Coordinate statBarSize {1, 26};
        static constexpr Coordinate hotbarSize {10, 26};
        static constexpr Coordinate chatBarSize {12, 55};
        /* Member Enums */
        enum class SubWindowType {
            MAIN, STATBAR, HOTBAR, CHATBAR, COUNT
        };
        /* Member Structs */
        struct Player {
            size_t hp;
            size_t curHotbarSlot;
        };
        /* Member Variables */
        std::vector<std::unique_ptr<WINDOW, WindowDeleter>> subwins;
        std::array<std::string, 10> hotbar;
        Player p;
        /* Private Member Methods */
        void initScreen();
        void drawStatBar();
        void hotbarSelect(size_t slot);
    public:
        /* Inherit Constructor from Screen */
        GameScreen();
        void drawGraphics();
        void updateScreen();
        void userInput(int key);
};

#endif // GAMESCREEN_H