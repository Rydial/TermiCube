#include "TermiCube.h"


int main() {

    GameWindow game;

    while(game.update() == 0);

    return 0;
}
  

// #include <iostream>
// #include <memory>
// #include <functional>

// class Button {

// };

// struct GameInfo {
//     Button btn;
// };

// class SharedGameInfo {
//     public:
//         std::shared_ptr<GameInfo> data;
// };

// class Game{
//     private:
//         GameInfo data;
//     public:
//         Game() : data{} {}
// };

// int main()
// {
//     Game game;
//     std::cout << sizeof(game) << '\n';
// }