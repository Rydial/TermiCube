#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <vector>
#include "Screen.h"

class GameScreen : public Screen {
    private:
        /* Member Constants */
        static constexpr size_t spriteWidth {2};
        static constexpr Coordinate mainSize {27, 82};
        static constexpr Coordinate statBarSize {1, 18};
        static constexpr Coordinate hotbarSize {5, 24};
        /* Member Enums */
        enum class SubWindowType {
            MAIN, STATBAR, HOTBAR, COUNT
        };
        /* Member Variables */
        std::vector<std::unique_ptr<WINDOW, WindowDeleter>> subwins;
        size_t hp {3};
        /* Private Member Methods */
        void initScreen();
        void drawStatBar();
    public:
        /* Inherit Constructor from Screen */
        GameScreen();
        void drawGraphics();
        void updateScreen();
        void userInput(int key);
};

#endif // GAMESCREEN_H