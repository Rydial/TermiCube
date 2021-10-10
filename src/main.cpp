#include "TermiCube.h"


int main() {

    GameWindow game;

    while(game.update() == 0);

    return 0;
}
  

// int main() {

//     initscr();
//     noecho();
//     cbreak();
    

//     if (curs_set(0) == ERR) {
//         addstr("Not working");
//     }
//     mvaddstr(1, 1, "Random sentence.");
//     refresh();

//     getch();
//     mvaddstr(2, 1, "Random sentence number two.");
//     getch();

//     endwin();

// }