#include <iostream>
#include "TermiCube.h"

int main() {

    std::cout << "[ Game Started ]\n\n";

    Game game {};

    while (game.getState() != GameState::END) {
        game.userInput();
        game.updateGame();
        game.drawGraphics();
    }
    
    std::cout << "\n[ Game Ended ]" << std::endl;

    return 0;
}
