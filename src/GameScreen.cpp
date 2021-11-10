#include <string>
#include "TermiCube.h"

/////////////////////////////////////* Base Class */////////////////////////////////////

GameScreen::GameScreen() :
    subwins{}
{
    /* Generate Subwindows */
    subwins.emplace_back(derwin(window.get(), mainSize.y, mainSize.x, 1, 1));
    subwins.emplace_back(derwin(window.get(), statBarSize.y, statBarSize.x,
        mainSize.y + 2, (maxCols - 1) - statBarSize.x));
    subwins.emplace_back(derwin(window.get(), hotbarSize.y, hotbarSize.x,
        mainSize.y + 2, 1));

    initScreen();
}

void GameScreen::initScreen()
{
    /* Screen Border */
    drawBorder();
    mvwadd_wch(window.get(), mainSize.y + 1, 0, &wchars[L"╠"]);
    mvwhline_set(window.get(), mainSize.y + 1, 1, &wchars[L"═"], mainSize.x);
    mvwadd_wch(window.get(), mainSize.y + 1, maxCols - 1, &wchars[L"╣"]);

    /* Main Sprites */
    // Draw entities

    /* Status Bar Border */
    mvwadd_wch(window.get(), mainSize.y + 1,
        (maxCols - 1) - statBarSize.x - 1, &wchars[L"╦"]);
    mvwvline_set(window.get(), mainSize.y + 2,
        (maxCols - 1) - statBarSize.x - 1, &wchars[L"║"], statBarSize.y);
    mvwadd_wch(window.get(), (mainSize.y + 2) + statBarSize.y,
        (maxCols - 1) - statBarSize.x - 1, &wchars[L"╚"]);
    mvwhline_set(window.get(), (mainSize.y + 2) + statBarSize.y,
        (maxCols - 1) - statBarSize.x, &wchars[L"═"], statBarSize.x);
    mvwadd_wch(window.get(), (mainSize.y + 2) + statBarSize.y,
        maxCols - 1, &wchars[L"╣"]);


    /* Status Bar Sprites */
    drawStatBar();

    /* Hotbar Border */
    mvwadd_wch(window.get(), mainSize.y + 1,
        hotbarSize.x + 1, &wchars[L"╦"]);
    mvwvline_set(window.get(), mainSize.y + 2,
        hotbarSize.x + 1, &wchars[L"║"], hotbarSize.y);
    mvwadd_wch(window.get(), (mainSize.y + 2) + hotbarSize.y,
        hotbarSize.x + 1, &wchars[L"╝"]);
    mvwhline_set(window.get(), (mainSize.y + 2) + hotbarSize.y,
        1, &wchars[L"═"], hotbarSize.x);
    mvwadd_wch(window.get(), (mainSize.y + 2) + hotbarSize.y, 0, &wchars[L"╠"]);

    // WINDOW *hotbarPtr {subwins[static_cast<size_t>(SubWindowType::HOTBAR)].get()};
    // mvwhline_set(hotbarPtr, 0, 1,  &wchars[L"─"], )
    // mvwaddstr(hotbarPtr, 0, 0, "Sword of Justice");
}

void GameScreen::drawStatBar()
{ /***** Line 1 in Notes.txt *****/
    WINDOW *statBarPtr {subwins[static_cast<size_t>(SubWindowType::STATBAR)].get()};
    /* Draw HP sprites */
    for (size_t x {(statBarSize.x - 1) - 2}, i {0}; i < hp; x -= spriteWidth, i++)
        mvwadd_wch(statBarPtr, 0, x, &wchars[L"❤️"]);
    /* Fill right side of rightmost heart with invisible characters*/
    for (size_t x {(statBarSize.x - 1) - 1}; x < statBarSize.x; x++)
        mvwadd_wch(statBarPtr, 0, x, &wchars[L" "]);
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