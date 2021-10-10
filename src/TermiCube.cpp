#include <string>
#include <fstream>
#include "TermiCube.h"


GameWindow::GameWindow() :
    screenList{},
    screen{ScreenType::MAINMENU}
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

Screen::Button::Button(WINDOW *win, int y, int x, int yLen, int xLen,
        std::function<void()> click, std::function<void()> draw) :
    ptr{win},
    yTop{y}, yBtm{y + yLen}, xLeft{x}, xRight{x + xLen},
    click{click}, draw{draw}
{

}

void Screen::Button::highlight(int attrs)
{
    wattron(ptr.get(), attrs);
    draw();
    wattroff(ptr.get(), attrs);
}

//////////////////////////////////////////////////////////////

MainMenuScreen::MainMenuScreen() :
    buttons{window.get(), btnStartPos.y, btnStartPos.x}
{
    initScreen();
}

void MainMenuScreen::initScreen()
{
    /* Title Creation */
    box(window.get(), 0 , 0);
    std::string path {"resource/mainmenu/title.txt"}, line;
    std::ifstream title {path};

    if (!title)
        std::cerr << "File could not be opened: " << path << '\n';
    
    for (size_t y {titlePos.y}, x{titlePos.x}; std::getline(title, line); y++)
        mvwaddstr(window.get(), y, x, line.c_str());

    /* Draw NEWGAME with current focus */
    buttons.list[buttons.index].highlight(COLOR_PAIR(1));
    /* Draw rest of the buttons */
    for (size_t i {1}; i < buttons.list.size(); i++)
        buttons.list[i].draw();
}

void MainMenuScreen::drawGraphics() 
{
	
}


void MainMenuScreen::updateScreen()
{

}



void MainMenuScreen::userInput(int key)
{   
    // mvwprintw(window.get(), 1, 1, "%d", key);

    if (key == control.up) {
        buttons.list[buttons.index].highlight(A_NORMAL);
        buttons.index = (buttons.index - 1) % buttons.list.size();
        buttons.list[buttons.index].highlight(COLOR_PAIR(1));
    } else if (key == control.down) {
        buttons.list[buttons.index].highlight(A_NORMAL);
        buttons.index = (buttons.index + 1) % buttons.list.size();
        buttons.list[buttons.index].highlight(COLOR_PAIR(1));
    }

    eData.key = key; /* Store key into event data */
    update_panels();
    doupdate();
}

MainMenuScreen::ButtonManager::ButtonManager(WINDOW *win, int startY, int startX) :
    list{},
    index{NEWGAME}
{
    /* NEWGAME */
    WINDOW *newGame {derwin(win, btnSize.y, btnSize.x, startY + 0, startX)};
    list.emplace_back(newGame, startY + 0, startX, btnSize.y, btnSize.x,
        []() {},
        [newGame]() {
            box(newGame, 0, 0);
            touchwin(newGame);
            wrefresh(newGame);
        }
    );
    /* LOADGAME */
    WINDOW *loadGame {derwin(win, btnSize.y, btnSize.x, startY + 6, startX)};
    list.emplace_back(loadGame, startY + 6, startX, btnSize.y, btnSize.x,
        []() {},
        [loadGame]() {
            box(loadGame, 0, 0);
            touchwin(loadGame);
            wrefresh(loadGame);
        }
    );
    /* SETTINGS */
    WINDOW *settings {derwin(win, btnSize.y, btnSize.x, startY + 12, startX)};
    list.emplace_back(settings, startY + 12, startX, btnSize.y, btnSize.x,
        []() {},
        [settings]() {
            box(settings, 0, 0);
            touchwin(settings);
            wrefresh(settings);
        }
    );
    /* CREDITS */
    WINDOW *credits {derwin(win, btnSize.y, btnSize.x, startY + 18, startX)};
    list.emplace_back(credits, startY + 18, startX, btnSize.y, btnSize.x,
        []() {},
        [credits]() {
            box(credits, 0, 0);
            touchwin(credits);
            wrefresh(credits);
        }
    );
}

//////////////////////////////////////////////////////////////

// void GameScreen::initScreen()
// {
    
// }

// void GameScreen::drawGraphics()
// {

// }


// void GameScreen::updateScreen()
// {

// }



// void GameScreen::userInput(int /*key*/)
// {

// }
