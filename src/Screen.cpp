#include <iostream>
#include <filesystem>
#include <bitset>
#include <fstream>
#include "Screen.h"
#include "TransferrableData.h"

//////////////////////////////////* Local Functions *//////////////////////////////////

namespace TC {

    namespace {

        size_t parseUTF8_Txt(std::vector<std::string> &dst, std::string path)
        {
            std::ifstream file {path};

            if (!file)
                std::cerr << "File could not be opened: " << path << '\n';

            using ULL = unsigned long long;
            std::string line;
            size_t maxLen {}, len {};

            while (std::getline(file, line)) {
                len = 0;

                for (size_t i {0}; i < line.length(); i++) {
                    if (line[i] == ' ')
                        len++;
                    /* In Windows a newline is represented in the CR + LF format: "\r\n" */
                    else if (line[i] != '\r') {
                        std::bitset<8> byte {static_cast<ULL>(line[i])};
                        size_t bits {0};

                        for (size_t j {0}; j < 8 && byte.test(7 - j); j++, bits++);

                        if (2 <= bits && bits <= 4) {
                            for (size_t k {0}, l {bits - 1}; k < l; k++) {
                                std::bitset<8> nextByte {static_cast<ULL>(line[++i])};

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

        Texts genTexts()
        {
            setlocale(LC_ALL, ""); /* Set terminal locale */
            namespace fs = std::filesystem;
            Texts texts {};
            std::vector<std::string> text {};
            size_t maxLen {};
            /* Recursively search "./resource" directory for .txt files */
            for (const auto &entry : fs::recursive_directory_iterator("resource")) {
                /* Parse .txt file and append content to texts */
                if (entry.path().extension() == ".txt") {
                    text.clear();
                    maxLen = {parseUTF8_Txt(text, entry.path().string())};
                    const auto &pair {std::make_pair<const std::vector<std::string>&,
                        const size_t&>(text, maxLen)};
                    texts.emplace(entry.path().stem().string(), pair);
                }
            }
            return texts;
        }

        WideChars genBoxDrawingChars()
        {
            std::ifstream file {"resource/widechar/BoxDrawing"};

            if (!file)
                std::cerr << "File could not be opened\n";

            WideChars wchars {};
            std::string mbChr {};
            wchar_t wChr[20] {};
            cchar_t cChr {};

            /* Store file content into multibyte strings */
            while (file >> mbChr) {
                if (mbChr.compare("//") == 0)
                    std::getline(file, mbChr);
                else {
                    /* Convert multibyte string to wide char string */
                    mbstowcs(wChr, mbChr.c_str(), 20);
                    /* Store wide char in cchar_t to be usable in ncurses functions */
                    setcchar(&cChr, wChr, 0, 0, nullptr);
                    wchars.emplace(std::wstring{wChr}, cChr);
                }
            }
            return wchars;
        }

        EntChars genEntityChars()
        {
            /* Prepare file stream */
            std::ifstream file {"resource/widechar/Entity"};

            if (!file)
                std::cerr << "File could not be opened\n";
        
            EntChars entChars {};
            std::string mbChr {};
            size_t index {};
            /* Store file content into multibyte strings vector in order */
            while (std::getline(file, mbChr)) {
                index = mbChr.find_last_of('[') + 1;
                entChars.emplace_back(mbChr.substr(index, mbChr.size() - index - 2));
            }

            return entChars;
        }

    }

}

//////////////////////////////////* Global Functions *//////////////////////////////////

size_t TC::pyMod(int n, int mod)
{   /* Haven't considered when mod is negative */
    if (n < 0)
        return static_cast<size_t>(mod - (abs(n) % mod));
    else
        return static_cast<size_t>(n % mod);
}


////////////////////////////////////* Screen *////////////////////////////////////

TC::Scr::Controls TC::Scr::control {'w', 'a', 's', 'd', '\n'};
TC::Scr::EventData TC::Scr::eData {0, {}};
const TC::Texts TC::texts {genTexts()};
const TC::WideChars TC::boxCh {genBoxDrawingChars()};
const TC::EntChars TC::entCh {genEntityChars()};

/*==============================================================================*/

TC::Scr::Screen() :
    window{newwin(maxRows, maxCols, (LINES - maxRows) / 2, (COLS - maxCols) / 2)},
    panel{new_panel(window.get())}
{
    
}

/*==============================================================================*/

void TC::Scr::drawBorder()
{
    wborder_set(window.get(), &boxCh.at(L"║"), &boxCh.at(L"║"),
        &boxCh.at(L"═"), &boxCh.at(L"═"), &boxCh.at(L"╔"),
        &boxCh.at(L"╗"), &boxCh.at(L"╚"), &boxCh.at(L"╝"));
}

////////////////////////////////////* BUTTON *////////////////////////////////////

TC::Btn::Button(WINDOW *win, int y, int x, int yLen, int xLen,
        const std::function<void()> &click,
        const std::function<void(WINDOW *)> &draw) :
    ptr{win},
    yTop{y}, yBtm{y + yLen}, xLeft{x}, xRight{x + xLen},
    click{click}, draw{draw}
{
    
}

/*==============================================================================*/

void TC::Btn::highlight(int attrs)
{
    wattron(ptr.get(), attrs);
    draw(ptr.get());
    wattroff(ptr.get(), attrs);
}

////////////////////////////////* BUTTON MANAGER *////////////////////////////////

TC::BtnMgr::ButtonManager(
    WINDOW *win, Size<> size, Point<> startPos, int offset,
    std::vector<std::string> txts, std::function<std::function<void()>(size_t)> genClick
) :
    list{},
    btn{0}
{
    int curY {0}, verSpacing {static_cast<int>(size.y) + offset};
    /* Generate Button + Subwindow */
    for (size_t i {0}; i < txts.size(); i++, curY += verSpacing) {
        list.emplace_back(
            derwin(win, size.y, size.x, startPos.y + curY, startPos.x),
            startPos.y + curY, startPos.x, size.y, size.x, genClick(i),
            genDrawFunc(texts.at(txts[i]).second, txts[i], size)
        );
    }
}

/*==============================================================================*/

std::function<void(WINDOW *)> TC::BtnMgr::genDrawFunc(
    size_t maxLen, const std::string &txt, Size<> btnSize)
{
    return [txt, maxLen, btnSize] (WINDOW *win) {
        box(win, 0, 0);

        for (size_t i {0}; i < texts.at(txt).first.size(); i++)
            mvwaddstr(win, i + 1, (btnSize.x - maxLen) / 2,
                texts.at(txt).first[i].c_str());
        
        wrefresh(win);
    };
}
