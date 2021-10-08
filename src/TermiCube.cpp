#include <iostream>
#include <string>
#include "TermiCube.h"


GameWindow::GameWindow() :
    screenList{},
    screen{ScreenType::MAINMENU},
    exit{false}
{
    initCurses();
    initScreens();
}

void GameWindow::initCurses()
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
}

void GameWindow::initScreens()
{
    /* unique_ptr are not copyable, and initializer lists only use copy
    semantics so emplace_back had to be used instead */
    /* Panel stack order from bottom to top */
    screenList.emplace_back(std::make_unique<GameScreen>());
    screenList.emplace_back(std::make_unique<MainMenuScreen>());
}

void GameWindow::update()
{
    screenList[static_cast<size_t>(screen)]->userInput();
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
    mvwprintw(panel_window(panel.get()), 5, 5, "═");
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
