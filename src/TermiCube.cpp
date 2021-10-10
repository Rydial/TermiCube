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
    ptr{derwin(win, yLen, xLen, y, x)},
    yTop{y}, yBtm{y + yLen}, xLeft{x}, xRight{x + xLen},
    click{click}, draw{draw}
{

}

void Screen::Button::highlight(int attrs)
{
    wattron(ptr.get(), attrs);
    box(ptr.get(), 0, 0);
    wattroff(ptr.get(), attrs);
    touchwin(ptr.get());
    wrefresh(ptr.get());
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

    /* Draw button borders */
    for (auto &button : buttons.list)
        box(button.ptr.get(), 0, 0);
    /* Start on new game button */
    buttons.list[buttons.index].highlight(COLOR_PAIR(1));
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
    list.emplace_back(win, startY + 0, startX, btnSize.y, btnSize.x,
        [](){}, [](){});
    list.emplace_back(win, startY + 6, startX, btnSize.y, btnSize.x,
        [](){}, [](){});
    list.emplace_back(win, startY + 12, startX, btnSize.y, btnSize.x,
        [](){}, [](){});
    list.emplace_back(win, startY + 18, startX, btnSize.y, btnSize.x,
        [](){}, [](){});
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
