#include <string>
#include "TermiCube.h"
#include "Utilites.h"


GameWindow::GameWindow() :
    screen{0},
    screenList{}
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
    screenList.emplace_back(std::make_unique<MainMenuScreen>(screen));
    screenList.emplace_back(std::make_unique<GameScreen>());
    /* Hide every screen except for starting screen (Main Menu) */
    for (size_t i {1}; i < screenList.size(); i++)
        hide_panel(screenList[i]->getPanel());
        
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

    screenList[screen]->userInput(key);
    screenList[screen]->updateScreen();
    screenList[screen]->drawGraphics();

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

MainMenuScreen::MainMenuScreen(size_t &curScreen) :
    buttons{window.get(), btnStartPos.y, btnStartPos.x, curScreen}
{
    initScreen();
}

void MainMenuScreen::initScreen()
{
    /* Title Creation */
    box(window.get(), 0 , 0);

    std::vector<std::string> title;
    parseUTF8(title, "resource/mainmenu/title.txt");

    for (size_t i {0}, y {titlePos.y}, x{titlePos.x}; i < title.size(); y++, i++)
        mvwaddstr(window.get(), y, x, title[i].c_str());

    /* Draw NEWGAME with current focus */
    buttons.list[buttons.btn].highlight(COLOR_PAIR(1));

    /* Draw rest of the buttons */
    for (size_t i {1}; i < buttons.list.size(); i++)
        buttons.list[i].draw();
}

void MainMenuScreen::drawGraphics() 
{
	update_panels();
    doupdate();
}


void MainMenuScreen::updateScreen()
{

}



void MainMenuScreen::userInput(int key)
{   
    if (key == control.up) {
        buttons.list[buttons.btn].highlight(A_NORMAL);
        buttons.btn = (buttons.btn - 1) % buttons.list.size();
        buttons.list[buttons.btn].highlight(COLOR_PAIR(1));
    } else if (key == control.down) {
        buttons.list[buttons.btn].highlight(A_NORMAL);
        buttons.btn = (buttons.btn + 1) % buttons.list.size();
        buttons.list[buttons.btn].highlight(COLOR_PAIR(1));
    }

    eData.key = key; /* Store key into event data */
}

MainMenuScreen::ButtonManager::ButtonManager(
  WINDOW *win, int startY, int startX, size_t &/*curScreen*/) :
    list{},
    btn{static_cast<size_t>(ButtonType::NEWGAME)}
{
    std::vector<std::string> paths {
        "resource/mainmenu/NewGameBtn.txt", "resource/mainmenu/LoadGameBtn.txt"
    };
    // size_t count {static_cast<size_t>(ButtonType::COUNT)};
    size_t count {2};
    int y {0};

    for (size_t i {0}; i < count; i++, y += 6) {
        /* Parse Button Txt Files */
        std::vector<std::string> txt;
        parseUTF8(txt, paths[i]);
        /* Generate Button + Subwindow */
        WINDOW *btnWin {derwin(win, btnSize.y, btnSize.x, startY + y, startX)};
        list.emplace_back(btnWin, startY + 0, startX, btnSize.y, btnSize.x,
            genClickFunction(win), genDrawFunction(btnWin, txt)
        );
    }
}

std::function<void()> MainMenuScreen::ButtonManager::genClickFunction(WINDOW *win)
{
    return [win] () {
        
    };
}

std::function<void()> MainMenuScreen::ButtonManager::genDrawFunction(
    WINDOW *win, std::vector<std::string> &txt)
{
    return [win, txt] () {
        box(win, 0, 0);

        for (size_t i {0}; i < txt.size(); i++) {
            mvwaddstr(win, i + 1,
                (btnSize.x - txt[i].length()) / 2, txt[i].c_str());
        }
        
        mvwaddstr(win, 4, 1, txt[0].c_str());
        // printf("  Diff: %ld", btnSize.x - txt[0].length());
        // printf("  BtnSize: %d", btnSize.x);
        // printf("  Length: %ld", txt[0].length());
        // printf("  Size: %ld", txt[0].size());
        // mvwaddstr(win, 1, 7, "▄  ▄ ▄▄▄ ▄   ▄   ▄▄▄▄  ▄▄  ▄   ▄ ▄▄▄");
        // mvwaddstr(win, 2, 7, "█▀▄█ █￭  █ ▄ █   █  ▄ █▄▄█ █▀▄▀█ █￭ ");
        // mvwaddstr(win, 3, 7, "▀  ▀ ▀▀▀  ▀▀▀    ▀▀▀▀ ▀  ▀ ▀   ▀ ▀▀▀");
        touchwin(win);
        wrefresh(win);
    };
}

//////////////////////////////////////////////////////////////

void GameScreen::initScreen()
{
    
}

void GameScreen::drawGraphics()
{

}


void GameScreen::updateScreen()
{

}



void GameScreen::userInput(int /*key*/)
{

}
