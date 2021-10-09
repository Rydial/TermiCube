#include "TermiCube.h"


int main() {

    GameWindow game;

    while(game.update() == 0);

    return 0;
}
  

// #include <curses.h>
// #include <stdio.h>
 
// int main()
// {
//   initscr();
//   cbreak();
//   noecho();

//   // Enables keypad mode. This makes (at least for me) mouse events getting
//   // reported as KEY_MOUSE, instead as of random letters.
//   keypad(stdscr, TRUE);

//   // Don't mask any mouse events
//   mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);


//   // echo -e "\e[?1000;1006;1015h"
//   // printf("\033[?1003h\n"); // Makes the terminal report mouse movement events
//   // fflush(stdout);
//   char temp[] {"\033[?1003h\n"};
//   putp(temp);
//   // printw("\033[?1003h\n");

//   for (;;) { 
//     int c = wgetch(stdscr);
 
//     // Exit the program on new line fed
//     if (c == '\n')
//       break;
 
//     char buffer[512];
//     size_t max_size = sizeof(buffer);
//     if (c == ERR) {
//       snprintf(buffer, max_size, "Nothing happened.");
//     }
//     else if (c == KEY_MOUSE) {
//       MEVENT event;
//       if (getmouse(&event) == OK) {
//         snprintf(buffer, max_size, "Mouse at row=%d, column=%d bstate=0x%08lx",
//                  event.y, event.x, event.bstate);
//       }
//       else {
//         snprintf(buffer, max_size, "Got bad mouse event.");
//       }
//     }
//     else {
//       // snprintf(buffer, max_size, "Mouse Event");
//       snprintf(buffer, max_size, "Pressed key %d (%s)", c, keyname(c));      
//     }
 
//     move(0, 0);
//     insertln();
//     addstr(buffer);
//     clrtoeol();
//     move(0, 0);
//   }
 
//   printf("\033[?1003l\n"); // Disable mouse movement events, as l = low

//   endwin();
 
//   return 0;
// }



// #include <ncurses.h>

// int main()
// {	initscr();			/* Start curses mode 		*/
// 	start_color();			/* Start color functionality	*/
	
// 	init_pair(1, COLOR_GREEN, COLOR_BLACK);
//   attron(COLOR_PAIR(1));
// 	// printw("A Big string which i didn't care to type fully ");
//   addstr("TEST ");
// 	// mvchgat(0, 0, -1, A_BLINK, 1, NULL);	
// 	/* 
// 	 * First two parameters specify the position at which to start 
// 	 * Third parameter number of characters to update. -1 means till 
// 	 * end of line
// 	 * Forth parameter is the normal attribute you wanted to give 
// 	 * to the charcter
// 	 * Fifth is the color index. It is the index given during init_pair()
// 	 * use 0 if you didn't want color
// 	 * Sixth one is always NULL 
// 	 */
// 	refresh();
//   getch();
// 	endwin();			/* End curses mode		  */
// 	return 0;
// }