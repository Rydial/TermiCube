#include "GameScreen.h"
#include <iostream>

/////////////////////////////////////* Base Class */////////////////////////////////////

GameScreen::GameScreen() :
    subwins{},
    hotbar{"Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven",
    "Eight", "Nine"},
    p{3, 1},
    focus{ScreenFocus::MAIN},
    chat{0, ""}
{
    /* Generate Subwindows */
    subwins.emplace_back(derwin(window.get(), mainSize.y, mainSize.x, 1, 1));
    subwins.emplace_back(derwin(window.get(), statBarSize.y, statBarSize.x,
        mainSize.y + 2, (maxCols - 1) - statBarSize.x));
    subwins.emplace_back(derwin(window.get(), hotbarSize.y, hotbarSize.x,
        (maxRows - 1) - hotbarSize.y, (maxCols - 1) - hotbarSize.x));
    subwins.emplace_back(derwin(window.get(), chatBarSize.y, chatBarSize.x,
        (maxRows - 1) - chatBarSize.y, 1));

    initScreen();
}

void GameScreen::initScreen()
{
    /* Screen Border */
    drawBorder();
    mvwadd_wch(window.get(), mainSize.y + 1, 0, &wchars[L"╠"]);
    mvwhline_set(window.get(), mainSize.y + 1, 1, &wchars[L"═"], mainSize.x);
    mvwadd_wch(window.get(), mainSize.y + 1, maxCols - 1, &wchars[L"╣"]);

    /* Status Bar Border */
    mvwadd_wch(window.get(), mainSize.y + 1,
        (maxCols - 1) - statBarSize.x - 1, &wchars[L"╦"]);
    mvwvline_set(window.get(), mainSize.y + 2,
        (maxCols - 1) - statBarSize.x - 1, &wchars[L"║"], statBarSize.y);
    mvwadd_wch(window.get(), (mainSize.y + 2) + statBarSize.y,
        (maxCols - 1) - statBarSize.x - 1, &wchars[L"╠"]);
    mvwhline_set(window.get(), (mainSize.y + 2) + statBarSize.y,
        (maxCols - 1) - statBarSize.x, &wchars[L"═"], statBarSize.x);
    mvwadd_wch(window.get(), (mainSize.y + 2) + statBarSize.y,
        maxCols - 1, &wchars[L"╣"]);
    /* Status Bar Sprites */
    drawStatBar();

    /* Hotbar Border */
    mvwvline_set(window.get(), (maxRows - 1) - hotbarSize.y,
        (maxCols - 1) - hotbarSize.x - 1, &wchars[L"║"], hotbarSize.y);
    mvwadd_wch(window.get(), maxRows - 1,
        (maxCols - 1) - hotbarSize.x - 1, &wchars[L"╩"]);
    mvwvline_set(window.get(), (maxRows - 1) - hotbarSize.y,
        (maxCols - 1) - hotbarSize.x + 3, &wchars[L"║"], hotbarSize.y);
    mvwadd_wch(window.get(), (maxRows - 1) - hotbarSize.y - 1,
        (maxCols - 1) - hotbarSize.x + 3, &wchars[L"╦"]);
    mvwadd_wch(window.get(), maxRows - 1,
        (maxCols - 1) - hotbarSize.x + 3, &wchars[L"╩"]);
    /* Hotbar Text */
    WINDOW *hotbarPtr {subwins[static_cast<size_t>(SubWindowType::HOTBAR)].get()};

    for (size_t i {0}; i < hotbar.size(); i++) {
        mvwaddstr(hotbarPtr, i, 5, hotbar[(i + 1) % 10].c_str());
        mvwprintw(hotbarPtr, i, 1, "%ld", (i + 1) % 10);
    }

    /* Chat Bar Border */
    mvwadd_wch(window.get(), (maxRows - 1) - 2, 0, &wchars[L"╠"]);
    mvwhline_set(window.get(), (maxRows - 1) - 2, 1, &wchars[L"═"], chatBarSize.x);
    mvwadd_wch(window.get(), (maxRows - 1) - 2, chatBarSize.x + 1, &wchars[L"╣"]);
    /* Chat Bar Text */
    mvwadd_wch(window.get(), (maxRows - 1) - 1, 2, &wchars[L"➔"]);
}

void GameScreen::drawStatBar()
{ /***** Line 1 in Notes.txt *****/
    WINDOW *statBarPtr {subwins[static_cast<size_t>(SubWindowType::STATBAR)].get()};
    /* Draw HP sprites */
    for (size_t x {(statBarSize.x - 1) - 2}, i {0}; i < p.hp; x -= spriteWidth, i++)
        mvwadd_wch(statBarPtr, 0, x, &wchars[L"❤️"]);
    /* Fill right side of rightmost heart with invisible characters*/
    for (size_t x {(statBarSize.x - 1) - 1}; x < statBarSize.x; x++)
        mvwadd_wch(statBarPtr, 0, x, &wchars[L" "]);
}

void GameScreen::hotbarSelect(size_t slot)
{ /* Select which hotbar slot to focus on (mimics keyboard arrangement) */
    WINDOW *hotbarPtr {subwins[static_cast<size_t>(SubWindowType::HOTBAR)].get()};
    size_t &curSlot {p.curHotbarSlot}, size {hotbar.size()};

    wattron(hotbarPtr, A_NORMAL);
    mvwaddstr(hotbarPtr, (curSlot + (size - 1)) % size, 5, hotbar[p.curHotbarSlot].c_str());
    mvwprintw(hotbarPtr, (curSlot + (size - 1)) % size, 1, "%ld", p.curHotbarSlot);
    wattroff(hotbarPtr, A_NORMAL);

    p.curHotbarSlot = slot;
    wattron(hotbarPtr, A_BOLD | COLOR_PAIR(2));
    mvwaddstr(hotbarPtr, (curSlot + (size - 1)) % size, 5, hotbar[p.curHotbarSlot].c_str());
    mvwprintw(hotbarPtr, (curSlot + (size - 1)) % size, 1, "%ld", p.curHotbarSlot);
    wattroff(hotbarPtr, A_BOLD | COLOR_PAIR(2));

    wrefresh(hotbarPtr);
}

void GameScreen::chatInput(int key)
{
    WINDOW *chatBarPtr {subwins[static_cast<size_t>(SubWindowType::CHATBAR)].get()};

    if (key == 27) { /* Escape Key */
        focus = ScreenFocus::MAIN;
        curs_set(0);
    } else if (' ' <= key && key <= '~') { /* Keyboard ASCII Characters */
        chat.curLine += key;
        mvwaddch(chatBarPtr, chatBarSize.y - 1, 4 + chat.cursorXPos++,
            static_cast<chtype>(key));
        wrefresh(chatBarPtr);
    } else if (key == 127) { /* Delete Key */
        if (!chat.curLine.empty()) {
            chat.curLine.resize(chat.curLine.size() - 1);
            mvwaddch(chatBarPtr, chatBarSize.y - 1, 4 + (--chat.cursorXPos), ' ');
            wmove(window.get(), (maxRows - 1) - 1, 5 + chat.cursorXPos);
            wrefresh(chatBarPtr);
        }
    }
}

void GameScreen::drawGraphics()
{
    
}

void GameScreen::updateScreen()
{

}

void GameScreen::userInput(int key)
{
    switch (focus) {
        case ScreenFocus::MAIN:
            if ('0' <= key && key <= '9')
                hotbarSelect(static_cast<size_t>(key - '0'));
            else if (key == '/') {
                focus = ScreenFocus::CHAT;
                /* Place Visible Cursor in Position */
                curs_set(1);
                wmove(window.get(), (maxRows - 1) - 1, 5 + chat.cursorXPos);
            }
            break;

        case ScreenFocus::CHAT:
            chatInput(key);
            break;

        case ScreenFocus::OPTIONS:
            break;
    }
}