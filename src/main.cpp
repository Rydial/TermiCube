#include "TermiCube.h"


int main() {

    GameWindow window;

    while (!window.gameEnded()) {
        window.update();
        break;
    }

    return 0;
}
