#include "TCWindow.h"


int main() {

    TCWindow game;

    while (!game.shouldClose())
        game.update();

    return 0;
}

// #include <iostream>
// #include <string>

// struct Point {
//     int x, y;
// };

// #include <vector>

// class Foo {
//     static auto bar() {
//         std::vector<int> ls;
//         return ls;
//     }
//     static const auto var = bar();
// };

// int main() {

// }


// class Foo {
//     private:
//         struct Console {
//             static constexpr Point c[2] {{5, 0}, {10, 0}};
//         } cnsl {};
//         enum Key : size_t {
//             ZERO, ONE
//         } key {Key::ZERO};
//         std::string str {"Testing"};
//         size_t index {0};

//         void update()
//         {   
//             // size_t tempIndex {0};
//             const auto &var {cnsl.c[static_cast<size_t>(Key::ZERO)]};
//             for (size_t i {0}; i < var.x; i++);
//             if (str.size() > var.x - 5) {}
//         }
//     public:
//         void run() {update();}
// };

// int main() {
//     Foo bar;
//     bar.run();
// }

#include <panel.h>
#include <iostream>

// int main()
// {
//     initscr();
//     cbreak();
//     noecho();

//     int key;
//     auto *ptr {newwin(10, 10, 10, 10)};

//     while ((key = wgetch(ptr)) != 'q') {
//         // nodelay(ptr, TRUE);

//         // if (wgetch(ptr) == 91) {
//         //     if (wgetch(ptr) == 67) {
//         //         std::cerr << "KEY_RIGHT\n";
//         //         nodelay(ptr, FALSE);
//         //         continue;
//         //     }
//         // }

//         // if ((key = wgetch(ptr)) == ERR)
//         //     key = 27;
//         // else if (key == 91) {
//         //     if (65 <= (key = wgetch(ptr)) && key <= 68) /* Arrow Keys */
//         //         std::cerr << "Yay\n";
//         // }
//         std::cerr << "Key: " << key << '\n';
//         // nodelay(ptr, FALSE);
//     }


//     endwin();
// }
