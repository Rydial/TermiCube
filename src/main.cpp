#include "TermiCube.h"


int main() {

    GameWindow window;

    while (!window.gameEnded()) {
        window.pollEvents();
        window.updateWindow();
        break;
    }

    return 0;
}