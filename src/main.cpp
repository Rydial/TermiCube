#include "TermiCube.h"


int main() {

    GameWindow game;

    while(game.update() == 0);

    return 0;
}
  

// #include <functional>
// #include <iostream>
// #include <memory>

// class Game;
// std::function<void(Game &)> func;
// int *ref;
// void *temp;

// struct GameInfo {
//     int screen2;
// };

// class Game {
//     private:
//         GameInfo info;
//         int screen;
//         void foo()
//         {
//             screen = 2;
//         }
//     public:
//         Game() :
//             info{21},
//             screen{1}
//         {
//             ref = {&screen};
//             func = {&Game::foo};
//             temp = {&info};
//         }
//         void print() {
//             std::cout << screen << '\n';
//         }
// };

// int main() {
//     Game game;
//     func(game);
//     game.print();
//     *ref = 5;
//     game.print();
//     std::cout << ((GameInfo *) temp)->screen2 << '\n';
// }
