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
    start_color(); /* Enable color functionality */
    raw(); /* Disable line buffering */
    noecho(); /* Disable input echoing */
    curs_set(0); /* Set cursor invisible */
    init_pair(1, COLOR_GREEN, COLOR_BLACK);

    /* Enable Mouse Events */
    // mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    // keypad(stdscr, TRUE);
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

void GameWindow::terminate() {
    endwin();
}

int GameWindow::update() 
{
    int key {getch()};

    if (key == 'q') {
        terminate();
        return 1;
    }

    screenList[static_cast<size_t>(screen)]->userInput(key);
    screenList[static_cast<size_t>(screen)]->updateScreen();
    screenList[static_cast<size_t>(screen)]->drawGraphics();

    return 0;
}

//////////////////////////////////////////////////////////////

/* Static Variable Initialization */
Screen::EventData Screen::eData {0, {}};
Screen::Controls Screen::control {'w', 'a', 's', 'd'};

Screen::Screen() :
    window{newwin(
        maxRows, maxCols,(LINES - maxRows) / 2,(COLS - maxCols) / 2)},
    panel{new_panel(window.get())}
{

}

void Screen::PanelDeleter::operator()(PANEL *ptr)
{
    del_panel(ptr);
}

void Screen::WindowDeleter::operator()(WINDOW *ptr)
{
    delwin(ptr);
}

//////////////////////////////////////////////////////////////

MainMenuScreen::MainMenuScreen() :
    /* Initialize buttons */
    newGameBtn{derwin(window.get(),
        btnSize.y, btnSize.x, newGame.yTop, newGame.xLeft)},
    loadGameBtn{derwin(window.get(),
        btnSize.y, btnSize.x, loadGame.yTop, loadGame.xLeft)},
    settingsBtn{derwin(window.get(),
        btnSize.y, btnSize.x, settings.yTop, settings.xLeft)},
    creditsBtn{derwin(window.get(),
        btnSize.y, btnSize.x, credits.yTop, credits.xLeft)}
{
    /* Title Creation */
    box(window.get(), 0 , 0);
    std::string path {"resource/mainmenu/title.txt"}, line;
    std::ifstream title {path};

    if (!title)
        std::cerr << "File could not be opened: " << path << '\n';
    
    for (size_t y {titlePos.y}, x{titlePos.x}; std::getline(title, line); y++)
        mvwaddstr(window.get(), y, x, line.c_str());

    /* Create Buttons */
    box(newGameBtn.get(), 0, 0);
    mvwaddstr(newGameBtn.get(), 1, 1, "Test");
    box(loadGameBtn.get(), 0, 0);
    box(settingsBtn.get(), 0, 0);
    box(creditsBtn.get(), 0, 0);
    /* Start on new game button */
    // wattron(newGameBtn.get(), COLOR_PAIR(1));
    
    // wattroff(newGameBtn.get(), COLOR_PAIR(1));
    // touchwin(newGameBtn.get());
    // wrefresh(newGameBtn.get());
}

void MainMenuScreen::drawGraphics() 
{
	
// }


// void MainMenuScreen::updateScreen()
// {

// }



void MainMenuScreen::userInput(int key)
{   
    // MEVENT event;
    mvwprintw(window.get(), 1, 1, "%d", key);
    update_panels();
    doupdate();

    /* Under construction */
	// if (key == KEY_MOUSE) {
        // if (getmouse(&event) == OK) {
            
        // } else
        //     std::cerr << "MEvent not retrievable\n";
    // }

    if (key == control.up) {

    } else if (key == control.down) {

    }

    eData.key = key; /* Store key into event data */
}

// }


// void GameScreen::updateScreen()
// {

// }



// void GameScreen::userInput(int /*key*/)
// {

// }
