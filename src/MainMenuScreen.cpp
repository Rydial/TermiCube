#include "MainMenuScreen.h"
#include "TransferrableData.h"

/////////////////////////////////////* Base Class */////////////////////////////////////

MainMenuScreen::MainMenuScreen(TCWindowSharedData &winSData) :
    buttons{window.get(), winSData}
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

MainMenuScreen::MMSButtonManager::MMSButtonManager(
        WINDOW *win, TCWindowSharedData &winSData)
{
    int curY {0};
    std::vector<std::string> txts {"NewGameBtn", "LoadGameBtn", "SettingsBtn", "ExitBtn"};
    /* Generate Button + Subwindow */
    for (size_t i {0}; i < txts.size(); i++, curY += 6) {
        list.emplace_back(
            derwin(win, btnSize.y, btnSize.x, btnStartPos.y + curY, btnStartPos.x),
            btnStartPos.y + curY, btnStartPos.x, btnSize.y, btnSize.x,
            genClickFunction(winSData, i),
            genDrawFunction(texts.at(txts[i]).second, txts[i], btnSize)
        );
    }
}

std::function<void()> MainMenuScreen::MMSButtonManager::genClickFunction(
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

