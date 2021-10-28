#include "TermiCube.h"


// int main() {

//     GameWindow game;

//     while(game.update() == 0);

//     return 0;
// }
  

#include <functional>
#include <iostream>


class Game {
    private:
        int screen;
        void foo()
        {
            screen = 2;
        }
    public:
        Game() :
            screen{1}
        {
        }
        void print() {
            std::cout << screen << '\n';
        }
};

int main() {
    Game game;
    // func(game);
    // game.print();
    // *ref = 5;
    game.print();
}
