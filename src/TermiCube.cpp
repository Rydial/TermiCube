#include <iostream>
#include <string>
#include <curses.h>
#include "TermiCube.h"


void GameWindow::initCurses()
{
    initscr(); /* Start curses mode */
    // cbreak();
    // noecho();
}

void GameWindow::pollEvents() 
{

}


void GameWindow::updateWindow() 
{
    screenList[static_cast<size_t>(screen)]->updateScreen();
    screenList[static_cast<size_t>(screen)]->drawGraphics();
}

//////////////////////////////////////////////////////////////

void MainMenuScreen::drawGraphics() 
{
    
    // std::cout << "\U00002550" << '\n';

    // for (int i {0}; i < 33; i++) {
    //     for (int j {0}; j < 33; j++) {

    //     }
    // }
		
	printw("Hello World !!!");	/* Print Hello World		  */
	refresh();			/* Print it on to the real screen */
	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */
}


void MainMenuScreen::updateScreen()
{
//     printw("Test");
//     refresh();
//     endwin();
}



void MainMenuScreen::userInput()
{

}


void GameScreen::drawGraphics()
{

}


//////////////////////////////////////////////////////////////

void GameScreen::updateScreen()
{

}



void GameScreen::userInput()
{

}
