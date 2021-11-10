#include <string>
#include "Screen.h"

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
