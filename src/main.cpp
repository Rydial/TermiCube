#include "TCWindow.h"


int main() {

    TCWindow game;

    while(game.update() == 0);

    return 0;
}


// int main()
// {
//     initscr();
//     cbreak();
//     noecho();

//     int key;
//     auto *ptr {newwin(10, 10, 10, 10)};

//     while ((key = wgetch(ptr)) != 'q') {
//         nodelay(ptr, TRUE);

//         // if (wgetch(ptr) == 91) {
//         //     if (wgetch(ptr) == 67) {
//         //         std::cerr << "KEY_RIGHT\n";
//         //         nodelay(ptr, FALSE);
//         //         continue;
//         //     }
//         // }

//         if ((key = wgetch(ptr)) == ERR)
//             key = 27;
//         else if (key == 91) {
//             if (65 <= (key = wgetch(ptr)) && key <= 68) /* Arrow Keys */
//                 std::cerr << "Yay\n";
//         }
//         std::cerr << "Key: " << key << '\n';
//         nodelay(ptr, FALSE);
//     }


//     endwin();
// }
