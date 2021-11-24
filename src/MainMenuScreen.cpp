#include "MainMenuScreen.h"

/////////////////////////////////////* Base Class */////////////////////////////////////

MainMenuScreen::MainMenuScreen(std::shared_ptr<TCWindowData> &winData) :
    buttons{window.get(), btnStartPos.y, btnStartPos.x, winData}
{
    initScreen();
}

void MainMenuScreen::initScreen()
{
    /* Title Creation */
    drawBorder();

    const auto &title {texts.at("Title").first};
    size_t xLen {texts.at("Title").second};

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

///////////////////////////////////* Button Manager *///////////////////////////////////

MainMenuScreen::ButtonManager::ButtonManager(
  WINDOW *win, int startY, int startX, std::shared_ptr<TCWindowData> &winData) :
    list{},
    btn{static_cast<size_t>(ButtonType::NEWGAME)}
{
    int count {static_cast<int>(ButtonType::COUNT)};
    std::vector<std::string> paths {
        "NewGameBtn", "LoadGameBtn", "SettingsBtn", "ExitBtn"};

    for (int i {0}, y {0}; i < count; i++, y += 6) {
        size_t maxLen {texts.at(paths[static_cast<size_t>(i)]).second};
        /* Generate Button + Subwindow */
        TCWindowSharedData winSData {winData};
        WINDOW *btnWin {derwin(win, btnSize.y, btnSize.x, startY + y, startX)};
        list.emplace_back(btnWin, startY + y, startX, btnSize.y, btnSize.x,
            genClickFunction(winSData, i),
            genDrawFunction(paths[static_cast<size_t>(i)], maxLen));
    }
}

std::function<void()> MainMenuScreen::ButtonManager::genClickFunction(
    TCWindowSharedData &winSData, int index)
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

std::function<void(WINDOW *)> MainMenuScreen::ButtonManager::genDrawFunction(
    const std::string &txt, size_t maxLen)
{
    return [txt, maxLen] (WINDOW *win) {
        box(win, 0, 0);

        for (size_t i {0}; i < texts.at(txt).first.size(); i++)
            mvwaddstr(win, i + 1, (btnSize.x - maxLen) / 2,
                texts.at(txt).first[i].c_str());
        
        wrefresh(win);
    };
}
