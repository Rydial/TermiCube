#include "TermiCube.h"
#include <string>
#include <fstream>
#include <iostream>
#include <bitset>

///////////////////////////////////* Local Functions *///////////////////////////////////

size_t parseUTF8(std::vector<std::string> &dst, std::string path)
{
    std::ifstream file {path};

    if (!file)
        std::cerr << "File could not be opened: " << path << '\n';

    std::string line;
    size_t maxLen {}, len {};

    while (std::getline(file, line)) {
        len = 0;

        for (size_t i {0}; i < line.length(); i++) {
            if (line[i] == ' ')
                len++;
            /* In Windows a newline is represented in the CR + LF format: "\r\n" */
            else if (line[i] != '\r') {
                std::bitset<8> byte {static_cast<unsigned long long>(line[i])};
                size_t bits {0};

                for (size_t j {0}; j < 8 && byte.test(7 - j); j++, bits++);

                if (2 <= bits && bits <= 4) {
                    for (size_t k {0}, l {bits - 1}; k < l; k++) {
                        std::bitset<8> nextByte {static_cast<unsigned long long>(line[++i])};

                        if (!nextByte.test(7) || nextByte.test(6)) {
                            std::cerr << "No continuation byte found\n";
                            exit(1);
                        }
                    }
                    len++;
                }
            }
        }
        dst.emplace_back(line);

        if (len > maxLen)
            maxLen = len;
    }
    return maxLen;
}

/////////////////////////////////////* Base Class */////////////////////////////////////

MainMenuScreen::MainMenuScreen(std::shared_ptr<TCWindowData> &winData) :
    buttons{window.get(), btnStartPos.y, btnStartPos.x, winData}
{
    initWideChars();
    initScreen();
}

void MainMenuScreen::initScreen()
{
    /* Title Creation */
    drawBorder();

    std::vector<std::string> title;
    size_t xLen {parseUTF8(title, "resource/mainmenu/title.txt")};

    for (size_t i {0}, y {titlePosY}; i < title.size(); y++, i++)
        mvwaddstr(window.get(), y, (maxCols - xLen) / 2, title[i].c_str());

    /* Draw NEWGAME with current focus */
    buttons.list[buttons.btn].highlight(COLOR_PAIR(1));

    /* Draw rest of the buttons */
    for (size_t i {1}; i < buttons.list.size(); i++)
        buttons.list[i].draw(buttons.list[i].ptr.get());
}

void MainMenuScreen::initWideChars()
{
    std::ifstream file {"resource/general/Unicode.txt"};
    std::string mbChr;
    wchar_t wChr[10];
    /* Store file content into multibyte strings */
    while (file >> mbChr) {
        /* Move to next line if comment symbol "//"" is found */
        if (mbChr.compare("//") == 0)
            std::getline(file, mbChr);
        else {
            /* Convert multibyte string to wide char string */
            mbstowcs(wChr, mbChr.c_str(), 10);
            /* Store wide char in cchar_t to be usable in ncurses functions */
            cchar_t cChr {};
            setcchar(&cChr, wChr, 0, 0, nullptr);
            wchars.emplace(std::wstring{wChr}, cChr);
        }
    }
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
        "resource/mainmenu/NewGameBtn.txt", "resource/mainmenu/LoadGameBtn.txt",
        "resource/mainmenu/SettingsBtn.txt", "resource/mainmenu/CreditsBtn.txt"
    };

    for (int i {0}, y {0}; i < count; i++, y += 6) {
        /* Parse Button Txt Files */
        std::vector<std::string> txt;
        size_t maxLineLen {parseUTF8(txt, paths[static_cast<size_t>(i)])};
        /* Generate Button + Subwindow */
        TCWindowSharedData winSData {winData};
        WINDOW *btnWin {derwin(win, btnSize.y, btnSize.x, startY + y, startX)};
        list.emplace_back(btnWin, startY + y, startX, btnSize.y, btnSize.x,
            genClickFunction(winSData, i), genDrawFunction(txt, maxLineLen)
        );
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
        default:
            return nullptr;
    }
}

std::function<void(WINDOW *)> MainMenuScreen::ButtonManager::genDrawFunction(
    std::vector<std::string> &txt, size_t maxLen)
{
    return [txt, maxLen] (WINDOW *win) {
        box(win, 0, 0);

        for (size_t i {0}; i < txt.size(); i++)
            mvwaddstr(win, i + 1, (btnSize.x - maxLen) / 2, txt[i].c_str());
        
        touchwin(win);
        wrefresh(win);
    };
}
