#include "Screen.h"
#include "MainMenuScreen.h"
#include "TransferrableData.h"

////////////////////////////////* MainMenu Screen *////////////////////////////////

TC::MMScr::MainMenuScreen(TC::WinSData &winSData) :
    btns{
        window.get(), btnSize, btnStartPos, 1,
        {"NewGame", "LoadGame", "Settings", "Exit"},
        [this, winSData] (int index) mutable {return genClickFunc(winSData, index);}
    }
{
    initScreen();
}

/*==============================================================================*/

void TC::MMScr::drawGraphics() 
{

}

std::function<void()> TC::MMScr::genClickFunc(TC::WinSData &winSData, int index)
{
    switch(index) {
        case static_cast<int>(ButtonType::NEWGAME):
            return [winSData] () mutable {
                winSData.switchScreen(static_cast<size_t>(ScreenType::GAME));
            };
        case static_cast<int>(ButtonType::EXIT):
            return [winSData] () mutable {
                winSData.terminate();
            };
        default:
            return nullptr;
    }
}

void TC::MMScr::initScreen()
{
    /* Title Creation */
    drawBorder();
    const auto &title {texts.at("Title").first};
    size_t xLen {texts.at("Title").second};

    for (size_t i {0}, y {titlePosY}; i < title.size(); y++, i++)
        mvwaddstr(window.get(), y, (maxCols - xLen) / 2, title[i].c_str());

    /* Draw NEWGAME with current focus */
    btns.list[btns.btn].highlight(COLOR_PAIR(1));

    /* Draw rest of the buttons */
    for (size_t i {1}; i < btns.list.size(); i++)
        btns.list[i].draw(btns.list[i].ptr.get());
}

void TC::MMScr::updateScreen()
{
    
}

void TC::MMScr::userInput(int key)
{   
    if (key == control.up) {
        btns.list[btns.btn].highlight(A_NORMAL);
        btns.btn = pyMod(static_cast<int>(btns.btn) - 1, std::ssize(btns.list));
        btns.list[btns.btn].highlight(COLOR_PAIR(1));
    } else if (key == control.down) {
        btns.list[btns.btn].highlight(A_NORMAL);
        btns.btn = (btns.btn + 1) % btns.list.size();
        btns.list[btns.btn].highlight(COLOR_PAIR(1));
    } else if (key == control.enter)
        btns.list[btns.btn].click();       

    eData.key = key; /* Store key into event data */
}
