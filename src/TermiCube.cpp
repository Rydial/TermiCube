#include <iostream>
#include <string>
#include "TermiCube.h"


void GameWindow::initCurses()
{
    initscr(); /* Start curses mode */
    cbreak();
    noecho();
    /* Stcscr needs to be updated everytime new window is created */
    refresh(); 
}

void GameWindow::initScreens()
{
    /* unique_ptr are not copyable, and initializer lists only use copy
    semantics so emplace_back had to be used instead */
    /* Panel stack order from bottom to top */
    // screenList.emplace_back(std::make_unique<GameScreen>());
    screenList.emplace_back(std::make_unique<MainMenuScreen>());
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

Screen::Screen() :
    panel{new_panel(newwin(
        rows, cols, (LINES - rows) / 2, (COLS - cols) / 2))}
{

}

//////////////////////////////////////////////////////////////

void MainMenuScreen::drawGraphics() 
{
	box(panel_window(panel.get()), 0 , 0);
    update_panels();
    doupdate();
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
