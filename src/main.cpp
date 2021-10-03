#include <iostream>
#include "TermiCube.h"


int main() {

    std::cout << "[ Game Started ]\n\n";

    GameWindow window;

    while (!window.gameEnded()) {
        window.pollEvents();
        window.updateWindow();
    }

    std::cout << "\n[ Game Ended ]" << std::endl;

    return 0;
}
