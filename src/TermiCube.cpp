#include <string>
#include <fstream>
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
    setlocale(LC_ALL, ""); /* Set terminal locale */
    initscr(); /* Start curses mode */
    cbreak(); /* Disable line buffering */
    noecho(); /* Disable input echoing */
    curs_set(0); /* Set cursor invisible */
}

void GameWindow::initScreens()
{
    /* unique_ptr are not copyable, and initializer lists only use copy
    semantics so emplace_back had to be used instead */
    /* Panel stack order from bottom to top */
    // screenList.emplace_back(std::make_unique<GameScreen>());
    screenList.emplace_back(std::make_unique<MainMenuScreen>());
    update_panels();
    doupdate();
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

void Screen::PanelDeleter::operator()(PANEL *ptr)
{
    del_panel(ptr);
}

void Screen::SubWindowDeleter::operator()(WINDOW *ptr)
{
    delwin(ptr);
}

//////////////////////////////////////////////////////////////

MainMenuScreen::MainMenuScreen()
{
    /* Title Creation */
    box(panel_window(panel.get()), 0 , 0);
    std::string path {"resource/title.txt"}, line;
    std::ifstream title {path};

    if (!title)
        std::cerr << "File could not be opened: " << path << '\n';
    
    for (size_t y {5}, x{4}; std::getline(title, line); y++)
        mvwaddstr(panel_window(panel.get()), y, x, line.c_str());
    /* New Game Button Creation */

}

void MainMenuScreen::drawGraphics() 
{
	// subwin
}


void MainMenuScreen::updateScreen()
{

}



void MainMenuScreen::userInput()
{
    getch();
<<<<<<< HEAD
	endwin();
=======
    endwin();
>>>>>>> 1a925e78c24d1143aad1c6b2a8b01d468cacc19f
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
