#include <string>
#include <iostream>
#include <fstream>
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
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
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
std::unordered_map<std::wstring, cchar_t> Screen::wchars {};

void Screen::drawBorder()
{
    wborder_set(window.get(), &wchars[L"║"], &wchars[L"║"], &wchars[L"═"],
    &wchars[L"═"], &wchars[L"╔"], &wchars[L"╗"], &wchars[L"╚"], &wchars[L"╝"]);
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
    initWideChars();
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

void MainMenuScreen::initWideChars()
{
    std::ifstream file {"resource/general/Unicode.txt"};
    std::string mbChr;
    wchar_t wChr[5];

    while (file >> mbChr) {
        mbstowcs(wChr, mbChr.c_str(), 5);
        cchar_t cChr {};
        setcchar(&cChr, wChr, 0, 0, nullptr);
        wchars.emplace(std::wstring{wChr}, cChr);
    }
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
    subwins.emplace_back(derwin(window.get(), mainSize.y, mainSize.x, 1, 1));
    subwins.emplace_back(derwin(window.get(), hpBarSize.y, hpBarSize.x,
        mainSize.y + 2, (maxCols - 1) - hpBarSize.x));

    initScreen();
}

void GameScreen::initScreen()
{
    /* Screen Border */
    drawBorder();
    mvwadd_wch(window.get(), mainSize.y + 1, 0, &wchars[L"╠"]);
    mvwhline_set(window.get(), mainSize.y + 1, 1, &wchars[L"═"], mainSize.x);
    mvwadd_wch(window.get(), mainSize.y + 1, maxCols - 1, &wchars[L"╣"]);

    /* Main Subwindow */
    // Draw entities

    /* HPBar Border */
    mvwadd_wch(window.get(), mainSize.y + 1,
        (maxCols - 1) - hpBarSize.x - 1, &wchars[L"╦"]);
    mvwvline_set(window.get(), mainSize.y + 2,
        (maxCols - 1) - hpBarSize.x - 1, &wchars[L"║"], 1);
    mvwadd_wch(window.get(), (mainSize.y + 2) + hpBarSize.y,
        (maxCols - 1) - hpBarSize.x - 1, &wchars[L"╚"]);
    mvwhline_set(window.get(), (mainSize.y + 2) + hpBarSize.y,
        (maxCols - 1) - hpBarSize.x, &wchars[L"═"], hpBarSize.x);
    mvwadd_wch(window.get(), (mainSize.y + 2) + hpBarSize.y,
        maxCols - 1, &wchars[L"╣"]);
    /* HPBar Subwindow */
    WINDOW *hpBarPtr {subwins[static_cast<size_t>(SubWindowType::HPBAR)].get()};
    /* The "Red Heart ❤️" character is 4 bytes long compared to the usual 2 bytes.
       So make sure there are at least 2 spaces between the characters */
    for (size_t x {hpBarSize.x - 4}, i {0}; i < hp ; x -= 4, i++) {
        std::cerr << "X: " << x << " I: " << i << '\n';
        mvwadd_wch(hpBarPtr, 0, x, &wchars[L"❤️"]);
    }
        
        
    touchwin(hpBarPtr);
    wrefresh(hpBarPtr);

    /* Hotbar Subwindow */
}

void GameScreen::drawGraphics()
{
//     WINDOW *mainPtr {subwins[static_cast<size_t>(SubWindowType::MAIN)].get()};
//     mvwhline_set(mainPtr, 0, 0, &wchars[L"🌲"], 41);
//     mvwvline_set(mainPtr, 0, 0, &wchars[L"🌲"], 27);
//     wrefresh(mainPtr);
}

void GameScreen::updateScreen()
{

}

void GameScreen::userInput(int /*key*/)
{

}
