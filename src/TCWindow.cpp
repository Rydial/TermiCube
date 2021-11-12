#include <locale>
#include "TCWindow.h"
#include "Screen.h"
#include "MainMenuScreen.h"
#include "GameScreen.h"


//////////////////////////////////* Transferrable Data *//////////////////////////////////

TCWindowSharedData::TCWindowSharedData(std::shared_ptr<TCWindowData> &gwData) :
    data{gwData}
{

}

void TCWindowSharedData::switchScreen(size_t index)
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

/////////////////////////////////////* Game Window */////////////////////////////////////

TCWindow::TermiCubeWindow() :
    data{std::make_shared<TCWindowData>(TCWindowData{0, {}})}
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
    refresh(); /* Line 14 on Notes.txt */
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
    init_pair(3, COLOR_WHITE, COLOR_YELLOW); /* Text Background */
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
    int key {wgetch(panel_window(data->screenList[data->screen]->getPanel()))};

    if (key == 'q') {
        terminate();
        return 1;
    }

    data->screenList[data->screen]->userInput(key);
    data->screenList[data->screen]->updateScreen();
    data->screenList[data->screen]->drawGraphics();

    return 0;
}