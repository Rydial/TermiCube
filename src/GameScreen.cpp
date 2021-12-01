#include <cmath>
#include <iostream>
#include <utility>
#include <entt/entt.hpp>
#include "Screen.h"
#include "GameScreen.h"
#include "TransferrableData.h"
#include "EntityComponents.h"


namespace TC {

    namespace {

        entt::registry reg;
        entt::observer velObs {reg, entt::collector.update<Vel>()};

    }

}

/////////////////////////////////////* GameScreen */////////////////////////////////////

TC::GScr::GameScreen(PANEL *panel, WinSData &winSData)    
  : subWins{},    hotbar{},
    cnsl{
        Console::Format::NORMAL,
        Console::Mode::INTEGRATED,
        {"", 0, 4, 0}, {}, {}
    },
    optMenu{
        std::unique_ptr<PANEL, PanelDel>(panel),
        [this] () {focus = ScreenFocus::MAIN;},
        {
            panel_window(panel), OptionMenu::btnSize,
            OptionMenu::btnStartPos, 2, {"Resume", "Settings", "MainMenu"},
            [this, winSData] (size_t index) mutable {
                return optMenu.genClickFunc(winSData, index);}
        }
    },    focus{ScreenFocus::MAIN},    info{{255, 256}, 5, 3, 1, 0},
    player{},
    map{std::vector<std::vector<std::vector<EID>>>(info.numOfLvls,
        std::vector<std::vector<EID>>(info.mapSize.y,
        std::vector<EID>(info.mapSize.x)))}
{
    
}

TC::GScr::GameScreen(TC::WinSData &winSData)
    : GameScreen(
        new_panel(newwin(OptionMenu::size.y, OptionMenu::size.x,
            ((static_cast<size_t>(LINES) - maxRows) / 2.0) + 1,
            (static_cast<size_t>(COLS) - OptionMenu::size.x) / 2)),
        winSData
    )
    
{
    initOptionMenu();
    initConsole();
    initSubWindows();
    initScreen();
    initEntities();
    drawMap();
}

/*==============================================================================*/

void TC::GScr::consoleInput(int key)
{
    WINDOW *ptr {subWins[static_cast<size_t>(SubWindowType::CONSOLE)].get()};
    const auto &size {cnsl.size[static_cast<size_t>(cnsl.mode)]};
    /////// ESC Key ///////
    if (key == 27) {
        focus = ScreenFocus::MAIN;
        curs_set(0);
    /////// Printable ASCII Characters ///////
    } else if (' ' <= key && key <= '~') {
        cnsl.input.line += key;
        /* Check if current line exceeds max line length */
        if (cnsl.input.line.size() > size.x - 5) {
            mvwaddstr(ptr, size.y - 1, 4, cnsl.input.line.substr(
                ++cnsl.input.cursIndex - (cnsl.input.cursPos - 4), size.x - 5).c_str());
        } else {
            ++cnsl.input.cursPos;
            mvwaddch(ptr, size.y - 1, 4 + cnsl.input.cursIndex++,
                static_cast<chtype>(key));
        }
    /////// DEL Key ///////
    } else if (key == 127 && !cnsl.input.line.empty()) {
        /* Check requirements */
        if (cnsl.input.cursIndex == 0 && !cnsl.input.highlight)
            return;
        int del {!cnsl.input.highlight ? 1 : cnsl.input.highlight};
        /* Update cursIndex */
        if (del > 0) {
            if (std::cmp_less(cnsl.input.cursIndex, del))
                cnsl.input.cursIndex = 0;
            else
                cnsl.input.cursIndex -= static_cast<size_t>(del);
        }
        /* Delete Target Substring */
        cnsl.input.line.erase(cnsl.input.cursIndex, static_cast<size_t>(abs(del)));
        cnsl.input.highlight = 0;
        /* Update cursPos */
        if (cnsl.input.cursIndex < size.x - 5)
            cnsl.input.cursPos = cnsl.input.cursIndex + 4;
        /* Update Current Line */
        if (cnsl.input.line.size() >= size.x - 5) { /* Overlength Delete */
            mvwaddstr(ptr, size.y - 1, 4, cnsl.input.line.substr(
                cnsl.input.cursIndex - (cnsl.input.cursPos - 4), size.x - 5).c_str());
        } else { /* Underlength Delete */
            mvwaddstr(ptr, size.y - 1, 4, cnsl.input.line.substr(
                0, cnsl.input.line.size()).c_str());
            /* Clear remaining characters */
            for (size_t i {0}; i < (size.x - 5) - cnsl.input.line.size(); ++i)
                waddch(ptr, ' ');
        }
        wmove(ptr, size.y - 1, cnsl.input.cursPos);
    /////// ENTER Key ///////
    } else if (key == control.enter) {
        /* Reformat Current Line and Add to Console Record */
        sendToConsole("➔  " + cnsl.input.line);
        /* Update Console Display */
        updateConsole();
        /* Clear Current Line on Console */
        cnsl.input.line.clear();
        cnsl.input.cursIndex = 0;
        wmove(ptr, size.y - 1, cnsl.input.cursPos = 4);
        wclrtoeol(ptr);

    } else if (key == KEY_RIGHT && cnsl.input.cursIndex < cnsl.input.line.size()) {
        ++cnsl.input.cursIndex;
        moveCursor(static_cast<int>(CursorMove::RIGHT));

    } else if (key == KEY_SRIGHT && cnsl.input.cursIndex < cnsl.input.line.size()) {
        ++cnsl.input.cursIndex;
        ++cnsl.input.highlight;
        moveCursor(static_cast<int>(CursorMove::RIGHT), true);

    } else if (key == KEY_LEFT && cnsl.input.cursIndex > 0) {
        --cnsl.input.cursIndex;
        moveCursor(static_cast<int>(CursorMove::LEFT));

    } else if (key == KEY_SLEFT && cnsl.input.cursIndex > 0) {
        --cnsl.input.cursIndex;
        --cnsl.input.highlight;
        moveCursor(static_cast<int>(CursorMove::LEFT), true);

    } else if (key == KEY_UP) {
        
    } else if (key == KEY_DOWN) {
        
    } else
        return;
    
    wrefresh(ptr);
}

void TC::GScr::drawGraphics()
{
    
}

void TC::GScr::drawMap()
{
    using namespace TC::EC;
    WINDOW *ptr {subWins[static_cast<size_t>(SubWindowType::MAIN)].get()};
    wmove(ptr, 0, 0);
    const auto &pos {reg.get<TC::EC::Pos>(player)};
    EID ent {};
    TC::Size<> tL {pos.y - (mainSize.y / 2), pos.x - (mainSize.x / 4)};
    /* Draw Map Centered on Player's Position */
    for (size_t y{0}; y < mainSize.y; ++y) {
        for (size_t x{0}; x < mainSize.x / 2; ++x) {
            ent = map[pos.z][tL.y + y][tL.x + x];
            /* Draw non-empty characters */
            waddstr(ptr, entCh[reg.get<Sprite>(ent).id].c_str());
        }
    }
    wrefresh(ptr);
}

void TC::GScr::drawStatBar()
{ /***** Line 1 in Notes.txt *****/
    WINDOW *ptr {subWins[static_cast<size_t>(SubWindowType::STATBAR)].get()};
    /* Draw HP sprites */
    for (size_t x {(statBarSize.x - 1) - 2}, i {0}; i < info.hp; x -= spriteWidth, ++i)
        mvwaddstr(ptr, 0, x, "❤️"); /* 💔 */
    /* Fill right side of rightmost heart with invisible characters*/
    for (size_t x {(statBarSize.x - 1) - 1}; x < statBarSize.x; ++x)
        mvwaddstr(ptr, 0, x, "⠀");
}

void TC::GScr::gameInput(int key)
{
    using namespace TC::EC;

    if (key == 27) { /* ESC Key */
        focus = ScreenFocus::OPTIONS;
        show_panel(optMenu.panel.get());
        update_panels();
        doupdate();
    } else if ('0' <= key && key <= '9')
        hotbarSelect(static_cast<size_t>(key - '0'));
    else if (key == '/') {
        focus = ScreenFocus::CONSOLE;
        /* Highlight Current Line */
        cnsl.input.highlight = std::ssize(cnsl.input.line);
        wattron(window.get(), COLOR_PAIR(3));
        const auto xLen {cnsl.size[static_cast<size_t>(cnsl.mode)].x};
        const auto pos {cnsl.input.line.size() > (xLen - 5) ?
            cnsl.input.line.size() - (xLen - 5) : 0};
        mvwaddstr(window.get(), (maxRows - 1) - 1, 5,
            cnsl.input.line.substr(pos).c_str());
        wattroff(window.get(), COLOR_PAIR(3));
        /* Turn Cursor Visible */
        curs_set(1);
    } else if (key == control.left) {
        reg.replace<Vel>(player, 0, 0, -1);
        reg.replace<Sprite>(player, 1);
    } else if (key == control.right) {
        reg.replace<Vel>(player, 0, 0, 1);
        reg.replace<Sprite>(player, 2);
    } else if (key == control.up) {
        reg.replace<Vel>(player, 0, -1, 0);
        reg.replace<Sprite>(player, 3);
    } else if (key == control.down) {
        reg.replace<Vel>(player, 0, 1, 0);
        reg.replace<Sprite>(player, 4);
    }
}

void TC::GScr::hotbarSelect(size_t slot)
{ /* Select which hotbar slot to focus on (mimics keyboard arrangement) */
    WINDOW *ptr {subWins[static_cast<size_t>(SubWindowType::HOTBAR)].get()};
    size_t &curSlot {info.hotbarSlot}, size {hotbar.size()};
    /* Unhighlight current hotbar slot */
    wattron(ptr, A_NORMAL);
    mvwaddstr(ptr, (curSlot + (size - 1)) % size, 5, hotbar[info.hotbarSlot].c_str());
    mvwprintw(ptr, (curSlot + (size - 1)) % size, 1, "%ld", info.hotbarSlot);
    wattroff(ptr, A_NORMAL);
    /* Highlight newly selected hotbar slot */
    info.hotbarSlot = slot;
    wattron(ptr, A_BOLD | COLOR_PAIR(2));
    mvwaddstr(ptr, (curSlot + (size - 1)) % size, 5, hotbar[info.hotbarSlot].c_str());
    mvwprintw(ptr, (curSlot + (size - 1)) % size, 1, "%ld", info.hotbarSlot);
    wattroff(ptr, A_BOLD | COLOR_PAIR(2));

    wrefresh(ptr);
}

void TC::GScr::initConsole()
{
    /* Setup Unbuffered File Stream for Console Output */
    std::string path {"build/log/test.log"};
    cnsl.file.rdbuf()->pubsetbuf(0, 0);
    cnsl.file.open(path); /* Write (Overwrite) Mode */
    // cnsl.file.open(path, std::ios::app); /* Append Mode */

    /* Check if file stream was opened */
    if (!cnsl.file)
        std::cerr << "File " << path << " could not be opened.\n";
}

void TC::GScr::initEntities()
{
    using namespace TC::EC;
    /* Set Empty Entity as EID:0 */
    reg.emplace<Sprite>(reg.create(), 0);
    /* Create player as EID:1*/
    player = {reg.create()};
    reg.emplace<Sprite>(player, 3);
    reg.emplace<Vel>(player);
    const auto &pos {reg.emplace<Pos>(player, info.curLvl,
        ((info.mapSize.y - 1) / 2), (info.mapSize.x - 1) / 2)};
    map[pos.z][pos.y][pos.x] = player;
    /* Tree Entity */
    reg.emplace<Sprite>(reg.create(), 5);
    map[pos.z][pos.y][pos.x + 8] = static_cast<EID>(2);
}

void TC::GScr::initOptionMenu()
{
    /* Draw RESUME with current focus */
    optMenu.btns.list[optMenu.btns.btn].highlight(COLOR_PAIR(1));

    /* Draw rest of the buttons */
    for (size_t i {1}; i < optMenu.btns.list.size(); i++)
        optMenu.btns.list[i].draw(optMenu.btns.list[i].ptr.get());

    /* Hide Option Menu */
    hide_panel(optMenu.panel.get());
    
}

void TC::GScr::initScreen()
{   
    /* Draw Screen Border */
    drawBorder();
    mvwadd_wch(window.get(), mainSize.y + 1, 0, &boxCh.at(L"╠"));
    mvwhline_set(window.get(), mainSize.y + 1, 1, &boxCh.at(L"═"), mainSize.x);
    mvwadd_wch(window.get(), mainSize.y + 1, maxCols - 1, &boxCh.at(L"╣"));

    /* Draw Status Bar Border */
    mvwadd_wch(window.get(), mainSize.y + 1,
        (maxCols - 1) - statBarSize.x - 1, &boxCh.at(L"╦"));
    mvwvline_set(window.get(), mainSize.y + 2,
        (maxCols - 1) - statBarSize.x - 1, &boxCh.at(L"║"), statBarSize.y);
    mvwadd_wch(window.get(), (mainSize.y + 2) + statBarSize.y,
        (maxCols - 1) - statBarSize.x - 1, &boxCh.at(L"╠"));
    mvwhline_set(window.get(), (mainSize.y + 2) + statBarSize.y,
        (maxCols - 1) - statBarSize.x, &boxCh.at(L"═"), statBarSize.x);
    mvwadd_wch(window.get(), (mainSize.y + 2) + statBarSize.y,
        maxCols - 1, &boxCh.at(L"╣"));
    /* Draw Status Bar Sprites */
    drawStatBar();

    /* Draw Hotbar Border */
    mvwvline_set(window.get(), (maxRows - 1) - hotbarSize.y,
        (maxCols - 1) - hotbarSize.x - 1, &boxCh.at(L"║"), hotbarSize.y);
    mvwadd_wch(window.get(), maxRows - 1,
        (maxCols - 1) - hotbarSize.x - 1, &boxCh.at(L"╩"));
    mvwvline_set(window.get(), (maxRows - 1) - hotbarSize.y,
        (maxCols - 1) - hotbarSize.x + 3, &boxCh.at(L"║"), hotbarSize.y);
    mvwadd_wch(window.get(), (maxRows - 1) - hotbarSize.y - 1,
        (maxCols - 1) - hotbarSize.x + 3, &boxCh.at(L"╦"));
    mvwadd_wch(window.get(), maxRows - 1,
        (maxCols - 1) - hotbarSize.x + 3, &boxCh.at(L"╩"));
    /* Draw Hotbar Text */
    WINDOW *hotbarPtr {subWins[static_cast<size_t>(SubWindowType::HOTBAR)].get()};

    for (size_t i {0}; i < hotbar.size(); ++i) {
        mvwaddstr(hotbarPtr, i, 5, hotbar[(i + 1) % 10].c_str());
        mvwprintw(hotbarPtr, i, 1, "%ld", (i + 1) % 10);
    }

    /* Draw Console Border */
    mvwadd_wch(window.get(), (maxRows - 1) - 2, 0, &boxCh.at(L"╠"));
    const auto cnslXLen {cnsl.size[static_cast<size_t>(cnsl.mode)].x};
    mvwhline_set(window.get(), (maxRows - 1) - 2, 1, &boxCh.at(L"═"), cnslXLen);
    mvwadd_wch(window.get(), (maxRows - 1) - 2, cnslXLen + 1, &boxCh.at(L"╣"));
    /* Draw Console Text */
    mvwaddstr(window.get(), (maxRows - 1) - 1, 2, "➔");
}

void TC::GScr::initSubWindows()
{   /* Generate Subwindows */
    subWins.emplace_back(derwin(window.get(), mainSize.y, mainSize.x, 1, 1));
    subWins.emplace_back(derwin(window.get(), statBarSize.y, statBarSize.x,
        mainSize.y + 2, (maxCols - 1) - statBarSize.x));
    subWins.emplace_back(derwin(window.get(), hotbarSize.y, hotbarSize.x,
        (maxRows - 1) - hotbarSize.y, (maxCols - 1) - hotbarSize.x));
    const auto &cnslSize {cnsl.size[static_cast<size_t>(cnsl.mode)]};
    subWins.emplace_back(derwin(window.get(), cnslSize.y, cnslSize.x,
        (maxRows - 1) - cnslSize.y, 1));
}

void TC::GScr::moveCursor(int side, bool highlight)
{
    WINDOW *ptr {subWins[static_cast<size_t>(SubWindowType::CONSOLE)].get()};
    const auto &size {cnsl.size[static_cast<size_t>(cnsl.mode)]};
    /* Update Console Input Values */
    if (side == -1 && cnsl.input.cursPos > 4)
        --cnsl.input.cursPos;
    else if (side == 1 && cnsl.input.cursPos < size.x - 1)
        ++cnsl.input.cursPos;
    /* Update Current Line */
    mvwaddstr(ptr, size.y - 1, 4, cnsl.input.line.substr(
        cnsl.input.cursIndex - (cnsl.input.cursPos - 4), size.x - 5).c_str());
    /* Highlight Target Substring */
    if (highlight) {
        size_t len {std::cmp_greater(abs(cnsl.input.highlight), size.x - 5) ?
            size.x - 5 : static_cast<size_t>(abs(cnsl.input.highlight))};
        
        if (cnsl.input.highlight < 0)
            mvwchgat(ptr, size.y - 1, cnsl.input.cursPos, len, 0, 3, nullptr);
        else
            mvwchgat(ptr, size.y - 1, cnsl.input.cursPos - static_cast<size_t>(
                cnsl.input.highlight), len, 0, 3, nullptr);
    }
    wmove(ptr, size.y - 1, cnsl.input.cursPos);
}

void TC::GScr::sendToConsole(std::string line)
{
    /* Append Line to Console Record */
    cnsl.record.emplace_back(line);
    /* Append Line to Console Log File */
    cnsl.file << line << '\n';   
}

void TC::GScr::updateConsole()
{
    WINDOW *ptr {subWins[static_cast<size_t>(SubWindowType::CONSOLE)].get()};
    const auto &size {cnsl.size[static_cast<size_t>(cnsl.mode)]};
    const Size<> max {size.y - 2, size.x - 2};
    std::vector<std::string> ls {};

    for (size_t i {0}, curI {}, newI {}, y {0}; i < cnsl.record.size(); ++i) {
        const auto &line {cnsl.record[(cnsl.record.size() - 1) - i]};
        curI = 0;
        /* Normal Formatting */
        while (true) {
            /* Substring is shorter than max length */
            if (max.x + curI >= line.size() - 2) {
                ls.emplace_back(line.substr(curI));
                break;
            } else {
                /* Check if substring consists of spaces */
                if ((newI = line.find_last_of(' ', curI + max.x)) == line.npos) {
                    ls.emplace_back(line.substr(curI, max.x));
                    curI += max.x;
                } else {
                    ls.emplace_back(line.substr(curI, newI - curI));
                    curI = newI + 1;
                }
            }
        }
        /* Print lines to console until line limit reached */
        while (!ls.empty()) {
            if (y > max.y)
                return;
            mvwaddstr(ptr, (max.y - 1) - y++, 1, ls.back().c_str());
            waddnstr(ptr, std::string(max.x, ' ').c_str(),
                max.x - (static_cast<size_t>(getcurx(ptr)) - 1));
            ls.pop_back();
        }       
    }
}

void TC::GScr::updateScreen()
{
    using namespace TC::EC;
    /* Iterate through living entities with updated velocities */
    for (const auto ent : velObs) {
        const auto &pos {reg.get<Pos>(ent)};
        const auto &vel {reg.get<Vel>(ent)};
        /* Check if entity is within Boundaries */
        if (withinBoundary(pos, vel)) {
            const auto newPos {pos + vel};
            /* Apply displacement if no entity collision */
            if (static_cast<size_t>(map[newPos.z][newPos.y][newPos.x]) == 0) {
                map[pos.z][pos.y][pos.x] = static_cast<EID>(0);
                map[newPos.z][newPos.y][newPos.x] = ent;
                reg.replace<Pos>(ent, newPos);
            }
        }
        // std::cerr << "Y:" << pos.y << " X:" << pos.x << '\n';
    }
    /* Update map if changes occured on current display */
    if (velObs.size() > 0) {
        drawMap();
        velObs.clear();
    }
}

void TC::GScr::userInput(int key)
{
    switch (focus) {
        case ScreenFocus::MAIN:
            gameInput(key);
            break;

        case ScreenFocus::CONSOLE:
            consoleInput(key);
            break;

        case ScreenFocus::OPTIONS:
            if (key == 27) { /* ESC Key */
                focus = ScreenFocus::MAIN;
                hide_panel(optMenu.panel.get());
                update_panels();
                doupdate();
            } else if (key == control.up) {
                optMenu.btns.list[optMenu.btns.btn].highlight(A_NORMAL);
                optMenu.btns.btn = pyMod(static_cast<int>(optMenu.btns.btn)
                    - 1, std::ssize(optMenu.btns.list));
                optMenu.btns.list[optMenu.btns.btn].highlight(COLOR_PAIR(1));
            } else if (key == control.down) {
                optMenu.btns.list[optMenu.btns.btn].highlight(A_NORMAL);
                optMenu.btns.btn = (optMenu.btns.btn + 1) % optMenu.btns.list.size();
                optMenu.btns.list[optMenu.btns.btn].highlight(COLOR_PAIR(1));
            } else if (key == control.enter)
                optMenu.btns.list[optMenu.btns.btn].click();       
            break;
    }
    eData.key = key; /* Store key into event data */
}

bool TC::GScr::withinBoundary(const Pos &pos, const Vel &vel)
{
    if ((pos.z == 0 && vel.z == -1) || (pos.z == info.numOfLvls - 1 && vel.z == 1))
        return false;
    if ((pos.y == 0 && vel.y == -1) || (pos.y == info.mapSize.y - 1 && vel.y == 1))
        return false;
    if ((pos.x == 0 && vel.x == -1) || (pos.x == info.mapSize.x - 1 && vel.x == 1))
        return false;

    return true;
}

/////////////////////////////////////* OptionMenu */////////////////////////////////////

std::function<void()> TC::GScr::OptionMenu::genClickFunc(
    TC::WinSData &winSData, size_t index)
{
    switch(index) {
        case static_cast<size_t>(ButtonType::RESUME):
            return [this] () {
                resetFocus();
                hide_panel(panel.get());
                update_panels();
                doupdate();
            };
        case static_cast<size_t>(ButtonType::SETTINGS):
            return [] () {
                /* Switch to settings panel */
            };
        case static_cast<size_t>(ButtonType::MAINMENU):
            return [this, winSData] () mutable {
                resetFocus();
                winSData.switchScreen(static_cast<size_t>(ScreenType::MAINMENU));
            };
        default:
            return nullptr;
    }
}
