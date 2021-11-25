#include <locale>
#include "TCWindow.h"
#include "TransferrableData.h"
#include "MainMenuScreen.h"
#include "GameScreen.h"


TCWindow::TermiCubeWindow() :
    data{std::make_shared<TCWindowData>(TCWindowData{false, 0, {}})}
{
    initCurses();
    initColors();
    initScreens();
}

TCWindow::~TermiCubeWindow()
{
    endwin();
}

void TCWindow::initCurses()
{
    initscr(); /* Start curses mode */
    raw(); /* Disable line buffering */
    noecho(); /* Disable input echoing */
    refresh(); /* Line 14 on Notes.txt */
    curs_set(0); /* Set cursor invisible */

    /* Enable Mouse Events */
    // mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
}

void TCWindow::initColors()
{
    start_color(); /* Enable color functionality */
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_BLUE, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_YELLOW); /* Text Background */
}

void TCWindow::initScreens()
{
    /* unique_ptr are not copyable, and initializer lists only use copy
    semantics so emplace_back had to be used instead */
    TCWindowSharedData winSData {data};
    data->screenList.emplace_back(std::make_unique<MainMenuScreen>(winSData));
    data->screenList.emplace_back(std::make_unique<GameScreen>());
    /* Hide every screen except for starting screen (Main Menu) */
    for (size_t i {1}; i < data->screenList.size(); i++)
        hide_panel(data->screenList[i]->getPanel());
    
    update_panels();
    doupdate();
}

bool TCWindow::shouldClose()
{
    return data->exit;
}

void TCWindow::update() 
{
    const auto &scr {data->screenList[data->screen]};
    const auto &winPtr {panel_window(scr->getPanel())};
    wtimeout(winPtr, -1); /* Blocking Mode */
    int key {wgetch(winPtr)};

    if (key == 'q') {
        data->exit = true;
        return;
    } else if (key == 27) { /* ESC Key */
        /* Non-Blocking Mode */
        wtimeout(winPtr, 0);
        /* Filter out disabled function keys */
        if ((key = wgetch(winPtr)) == ERR)
            key = 27;
        else if (key == 91) { /* Function Keys (Consider making key map) */
            if ((key = wgetch(winPtr)) == 67)
                key = KEY_RIGHT;
            else if (key == 68)
                key = KEY_LEFT;
            else if (key == 65)
                key = KEY_UP;
            else if (key == 66)
                key = KEY_DOWN;
            else if (key == 49) {
                if (wgetch(winPtr) == 59 && wgetch(winPtr) == 50) {
                    if ((key = wgetch(winPtr)) == 67)
                        key = KEY_SRIGHT;
                    else if (key == 68)
                        key = KEY_SLEFT;
                    else
                        return;
                } else
                    return;
            } else
                return;
        } else
            return;
    }
    scr->userInput(key);
    scr->updateScreen();
    scr->drawGraphics();
}