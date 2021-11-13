#include <cmath>
#include <iostream>
#include "GameScreen.h"

/////////////////////////////////////* Base Class */////////////////////////////////////

GameScreen::GameScreen() :
    subwins{},
    hotbar{"Zero", "One", "Two", "Three", "Four", "Five", "Six", "Seven",
    "Eight", "Nine"},
    p{3, 1},
    focus{ScreenFocus::MAIN},
    cnsl{{"", 5, 0}, {}, {}}
{
    /* Setup Unbuffered File Stream for Console Output */
    std::string path {"build/log/test.log"};
    cnsl.file.rdbuf()->pubsetbuf(0, 0);
    cnsl.file.open(path); /* Write (Overwrite) Mode */
    // cnsl.file.open(path, std::ios::app); /* Append Mode */

    if (!cnsl.file)
        std::cerr << "File " << path << " could not be opened.\n";
    /* Generate Subwindows */
    subwins.emplace_back(derwin(window.get(), mainSize.y, mainSize.x, 1, 1));
    subwins.emplace_back(derwin(window.get(), statBarSize.y, statBarSize.x,
        mainSize.y + 2, (maxCols - 1) - statBarSize.x));
    subwins.emplace_back(derwin(window.get(), hotbarSize.y, hotbarSize.x,
        (maxRows - 1) - hotbarSize.y, (maxCols - 1) - hotbarSize.x));
    subwins.emplace_back(derwin(window.get(), consoleSize.y, consoleSize.x,
        (maxRows - 1) - consoleSize.y, 1));

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

    /* Console Border */
    mvwadd_wch(window.get(), (maxRows - 1) - 2, 0, &wchars[L"╠"]);
    mvwhline_set(window.get(), (maxRows - 1) - 2, 1, &wchars[L"═"], consoleSize.x);
    mvwadd_wch(window.get(), (maxRows - 1) - 2, consoleSize.x + 1, &wchars[L"╣"]);
    /* Console Text */
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

void GameScreen::consoleInput(int key)
{
    WINDOW *consolePtr {subwins[static_cast<size_t>(SubWindowType::CONSOLE)].get()};

    if (key == 27) { /* ESC Key */
        focus = ScreenFocus::MAIN;
        curs_set(0);
    } else if (' ' <= key && key <= '~') { /* Printable ASCII Characters */
        cnsl.input.str += key;
        /* Check if current line exceeds max line length */
        if (cnsl.input.str.size() > consoleSize.x - 5) {
            mvwaddstr(consolePtr, consoleSize.y - 1, 4, cnsl.input.str.substr(
                cnsl.input.str.size() - (consoleSize.x - 5), consoleSize.x - 5).c_str());
        } else {
            mvwaddch(consolePtr, consoleSize.y - 1, cnsl.input.cursPos++ - 1,
                static_cast<chtype>(key));
        }
        wrefresh(consolePtr);
    } else if (key == 127 && !cnsl.input.str.empty()) { /* DEL Key */
        int del {!cnsl.input.highlight ? 1 : cnsl.input.highlight};
        int cursPos {static_cast<int>(cnsl.input.str.size())};
        int pos {del > 0 ? cursPos - del : cursPos};
        cnsl.input.str.erase(static_cast<size_t>(pos), static_cast<size_t>(del));
        cnsl.input.highlight = 0;
        /* Delete specified substring */
        if (cnsl.input.str.size() >= consoleSize.x - 5) { /* Overlength Delete */
            mvwaddstr(consolePtr, consoleSize.y - 1, 4, cnsl.input.str.substr(
                cnsl.input.str.size() - (consoleSize.x - 5), consoleSize.x - 5).c_str());
        } else if (del == 1) { /* Underlength Single Delete */
            mvwaddch(consolePtr, consoleSize.y - 1, --cnsl.input.cursPos - 1, ' ');
            wmove(window.get(), (maxRows - 1) - 1, cnsl.input.cursPos);
        } else { /* Underlength Highlight Delete */
            cnsl.input.cursPos = static_cast<size_t>(pos) + 5;
            const auto &subStr {cnsl.input.str.substr(static_cast<size_t>(pos))};
            mvwaddstr(consolePtr, consoleSize.y - 1, cnsl.input.cursPos - 1, subStr.c_str());
            wclrtoeol(consolePtr);
            wmove(consolePtr, consoleSize.y - 1, cnsl.input.cursPos - 1);
        }
        wrefresh(consolePtr);
    } else if (key == control.enter) { /* ENTER Key */
        /* Reformat Current Line and Add to Console Record */
        sendToConsole(cnsl.input.str, L"➔");
        /* Update Console Display */
        updateConsole();
        /* Clear Current Line on Console */
        cnsl.input.str.clear();
        cnsl.input.cursPos = 5;
        wmove(consolePtr, consoleSize.y - 1, cnsl.input.cursPos - 1);
        wclrtoeol(consolePtr);
        wrefresh(consolePtr);
    } else if (key == KEY_RIGHT) {
        std::cerr << "RIGHT\n";
    } else if (key == KEY_LEFT) {
        std::cerr << "LEFT\n";
    } else if (key == KEY_UP) {
        std::cerr << "UP\n";
    } else if (key == KEY_DOWN) {
        std::cerr << "DOWN\n";
    }
}

void GameScreen::sendToConsole(std::string str, const std::wstring &icon)
{
    size_t pos {0}, newPos {0};
    /* Place newline to prevent cutoff words at max line length */
    while (true) {
        if ((newPos = str.find(" ", newPos)) == std::string::npos) {
            if (pos % (consoleSize.x - 5) > (str.size() - 1) % (consoleSize.x - 5))
                str.replace(pos, 1, "\n");
            break;
        } else {
            if (pos % (consoleSize.x - 5) > newPos % (consoleSize.x - 5))
                str.replace(pos, 1, "\n");
            pos = newPos++;
        }
    }
    /* Append Line to Console Record */
    cnsl.record.emplace_back(std::make_pair(str, icon));
    /* Append Line to Console Log File */
    for (size_t i {0}; (i = str.find('\n', i)) != std::string::npos;)
        str.insert(++i, 4, ' ');

    char arr[10];
    wcstombs(arr, icon.c_str(), 10);
    cnsl.file << ' ' << arr << ' ' << str << '\n';
}

void GameScreen::updateConsole()
{
    WINDOW *consolePtr {subwins[static_cast<size_t>(SubWindowType::CONSOLE)].get()};

    for (size_t i {0}, lineNum {0}, pos {}, newPos {}; i < cnsl.record.size(); i++) {
        auto &line {cnsl.record[cnsl.record.size() - 1 - i]};
        pos = line.first.size() - 1;

        while (lineNum < consoleSize.y - 2) {
            newPos = line.first.find_last_of('\n', pos);
            wmove(consolePtr, (consoleSize.y - 3) - lineNum, 1);
            wclrtoeol(consolePtr);

            if (newPos == std::string::npos) {
                mvwaddwstr(consolePtr, (consoleSize.y - 3) - lineNum, 1, line.second.c_str());
                mvwaddstr(consolePtr, (consoleSize.y - 3) - lineNum++, 4,
                    line.first.substr(0, pos + 1).c_str());
                break;
            } else {
                mvwaddstr(consolePtr, (consoleSize.y - 3) - lineNum++, 4,
                    line.first.substr(newPos + 1, pos - newPos).c_str());
                pos = newPos - 1;
            }
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
                focus = ScreenFocus::CONSOLE;
                /* Highlight Current Line */
                cnsl.input.highlight = static_cast<int>(cnsl.input.str.size());
                wattron(window.get(), COLOR_PAIR(3));
                mvwaddstr(window.get(), (maxRows - 1) - 1, 5, cnsl.input.str.c_str());
                wattroff(window.get(), COLOR_PAIR(3));
                /* Turn Cursor Visible */
                curs_set(1);
            }
            break;

        case ScreenFocus::CONSOLE:
            std::cerr << "Key: " << key << '\n';
            consoleInput(key);
            break;

        case ScreenFocus::OPTIONS:
            break;
    }
}