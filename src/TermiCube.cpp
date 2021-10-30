#include <string>
#include <iostream>
#include "TermiCube.h"
#include "Utilites.h"


GameWindowSharedData::GameWindowSharedData(std::shared_ptr<GameWindowData> &gwData) :
    data{gwData}
{

}

void GameWindowSharedData::switchScreen(size_t index)
{
    auto gwData {data.lock()};
    /* Checks if object is still available */
    if (gwData) {
        hide_panel(gwData->screenList[gwData->screen]->getPanel());
        gwData->screen = index;
        show_panel(gwData->screenList[gwData->screen]->getPanel());
        update_panels();
        doupdate();
    }
}

//////////////////////////////////////////////////////////////

TCWindow::TermiCubeWindow() :
    data{std::make_shared<GameWindowData>(GameWindowData{0, {}})}
{
    initCurses();
    initColors();
    initWideChars();
    initScreens();
}

void TCWindow::initCurses()
{
    setlocale(LC_ALL, ""); /* Set terminal locale */
    initscr(); /* Start curses mode */
    raw(); /* Disable line buffering */
    // cbreak();
    noecho(); /* Disable input echoing */
    curs_set(0); /* Set cursor invisible */

    /* Enable Mouse Events */
    // mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    // keypad(stdscr, TRUE);
}

void TCWindow::initColors()
{
    start_color(); /* Enable color functionality */
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
}

void TCWindow::initWideChars()
{
    // setcchar()
}

void TCWindow::initScreens()
{
    /* unique_ptr are not copyable, and initializer lists only use copy
    semantics so emplace_back had to be used instead */
    data->screenList.emplace_back(std::make_unique<MainMenuScreen>(data));
    data->screenList.emplace_back(std::make_unique<GameScreen>());
    /* Hide every screen except for starting screen (Main Menu) */
    for (size_t i {1}; i < data->screenList.size(); i++)
        hide_panel(data->screenList[i]->getPanel());
        
    update_panels();
    doupdate();
}

void TCWindow::terminate() {
    endwin();
}

int TCWindow::update() 
{
    int key {getch()};

    if (key == 'q') {
        terminate();
        return 1;
    }

    data->screenList[data->screen]->userInput(key);
    data->screenList[data->screen]->updateScreen();
    data->screenList[data->screen]->drawGraphics();

    return 0;
}

//////////////////////////////////////////////////////////////

/* Static Variable Initialization */
Screen::EventData Screen::eData {0, {}};
Screen::Controls Screen::control {'w', 'a', 's', 'd', '\n'};

void Screen::drawBorder()
{
    wborder_set(window.get(), 0, 0, 0, 0, 0, 0, 0, 0);
}

Screen::Screen() :
    window{newwin(maxRows, maxCols, (LINES - maxRows) / 2, (COLS - maxCols) / 2)},
    panel{new_panel(window.get())}
{

}

Screen::Button::Button(WINDOW *win, int y, int x, int yLen, int xLen,
        std::function<void()> click, std::function<void(WINDOW *)> draw) :
    ptr{win},
    yTop{y}, yBtm{y + yLen}, xLeft{x}, xRight{x + xLen},
    click{click}, draw{draw}
{
    
}

void Screen::Button::highlight(int attrs)
{
    wattron(ptr.get(), attrs);
    draw(ptr.get());
    wattroff(ptr.get(), attrs);
}

//////////////////////////////////////////////////////////////

MainMenuScreen::MainMenuScreen(std::shared_ptr<GameWindowData> &gwData) :
    buttons{window.get(), btnStartPos.y, btnStartPos.x, gwData}
{
    initScreen();
}

void MainMenuScreen::initScreen()
{
    /* Title Creation */
    drawBorder();

    std::vector<std::string> title;
    size_t xLen {parseUTF8(title, "resource/mainmenu/title.txt")};

    for (size_t i {0}, y {titlePosY}; i < title.size(); y++, i++)
        mvwaddstr(window.get(), y, (maxCols - xLen) / 2, title[i].c_str());

    /* Draw NEWGAME with current focus */
    buttons.list[buttons.btn].highlight(COLOR_PAIR(1));

    /* Draw rest of the buttons */
    for (size_t i {1}; i < buttons.list.size(); i++)
        buttons.list[i].draw(buttons.list[i].ptr.get());
}

void MainMenuScreen::drawGraphics() 
{

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
    } else if (key == control.enter)
        buttons.list[buttons.btn].click();       

    eData.key = key; /* Store key into event data */
}

MainMenuScreen::ButtonManager::ButtonManager(
  WINDOW *win, int startY, int startX, std::shared_ptr<GameWindowData> &gwData) :
    list{},
    btn{static_cast<size_t>(ButtonType::NEWGAME)}
{
    int count {static_cast<int>(ButtonType::COUNT)};
    std::vector<std::string> paths {
        "resource/mainmenu/NewGameBtn.txt", "resource/mainmenu/LoadGameBtn.txt",
        "resource/mainmenu/SettingsBtn.txt", "resource/mainmenu/CreditsBtn.txt"
    };

    for (int i {0}, y {0}; i < count; i++, y += 6) {
        /* Parse Button Txt Files */
        std::vector<std::string> txt;
        size_t maxLineLen {parseUTF8(txt, paths[static_cast<size_t>(i)])};
        /* Generate Button + Subwindow */
        GameWindowSharedData gwSData {gwData};
        WINDOW *btnWin {derwin(win, btnSize.y, btnSize.x, startY + y, startX)};
        list.emplace_back(btnWin, startY + y, startX, btnSize.y, btnSize.x,
            genClickFunction(gwSData, i), genDrawFunction(txt, maxLineLen)
        );
    }
}

std::function<void()> MainMenuScreen::ButtonManager::genClickFunction(
    GameWindowSharedData &gwSData, int index)
{
    switch(index) {
        case static_cast<int>(ButtonType::NEWGAME):
            return [gwSData] () mutable {
                gwSData.switchScreen(static_cast<size_t>(ScreenType::GAME));
            };
        default:
            return nullptr;
    }
}

std::function<void(WINDOW *)> MainMenuScreen::ButtonManager::genDrawFunction(
    std::vector<std::string> &txt, size_t maxLen)
{
    return [txt, maxLen] (WINDOW *win) {
        box(win, 0, 0);

        for (size_t i {0}; i < txt.size(); i++)
            mvwaddstr(win, i + 1, (btnSize.x - maxLen) / 2, txt[i].c_str());
        
        touchwin(win);
        wrefresh(win);
    };
}

//////////////////////////////////////////////////////////////

GameScreen::GameScreen() :
    subwins{}
{
    /* Generate Subwindows */
    subwins.emplace_back(derwin(window.get(), 5, 40, 4, 4));
    subwins.emplace_back(derwin(window.get(),
        hotbarSize.y, hotbarSize.x, 30, (maxCols - hotbarSize.x) / 2));

    initScreen();
}

void GameScreen::initScreen()
{
    /* Screen Border */
    drawBorder();
    /* Main Subwindow */

    /* Hotbar Subwindow */
    WINDOW *hotbarPtr {subwins[static_cast<size_t>(SubWindowType::HOTBAR)].get()};
    box(hotbarPtr, 0, 0);

    for (size_t x {6}; x < hotbarSize.x; x += 6) {
        // mvwvline(hotbarPtr, 1, x, '│', 3);
        // 
    }    
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
