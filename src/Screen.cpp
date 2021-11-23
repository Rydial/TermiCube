#include <iostream>
#include <filesystem>
#include <bitset>
#include <fstream>
#include "Screen.h"


/////////////////////////////////// Local Functions ///////////////////////////////////

size_t parseUTF8_Txt(std::vector<std::string> &dst, std::string path)
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

///////////////////////////////////// SCREEN /////////////////////////////////////

using Texts = std::unordered_map<std::string, std::pair<std::vector<std::string>, size_t>>;
using WideChars = std::unordered_map<std::wstring, cchar_t>;

/*==============================================================================*/

Screen::Controls Screen::control {'w', 'a', 's', 'd', '\n'};
Screen::EventData Screen::eData {0, {}};
const Texts Screen::texts {genTexts()};
// WideChars Screen::wchars {};
const WideChars Screen::wchars {genWideChars()};

/*==============================================================================*/

Screen::Screen() :
    window{newwin(maxRows, maxCols, (LINES - maxRows) / 2, (COLS - maxCols) / 2)},
    panel{new_panel(window.get())}
{

}

/*==============================================================================*/

void Screen::drawBorder()
{
    std::cerr << wchars.size() << '\n';
    wborder_set(window.get(), &wchars.at(L"║"), &wchars.at(L"║"),
        &wchars.at(L"═"), &wchars.at(L"═"), &wchars.at(L"╔"),
        &wchars.at(L"╗"), &wchars.at(L"╚"), &wchars.at(L"╝"));
    std::cerr << "BYE!\n";
}

Texts Screen::genTexts()
{
    namespace fs = std::filesystem;
    Texts texts;
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

WideChars Screen::genWideChars()
{
    std::ifstream file {"resource/general/Unicode"};

    if (!file)
        std::cerr << "File could not be opened\n";

    std::unordered_map<std::wstring, cchar_t> temp;
    std::string mbChr;
    wchar_t wChr[10] {};
    /* Store file content into multibyte strings */
    while (file >> mbChr) {
        std::cerr << "Size: " << temp.size() << '\n';
        /* Move to next line if comment symbol "//"" is found */
        if (mbChr.compare("//") == 0)
            std::getline(file, mbChr);
        else {
            /* Convert multibyte string to wide char string */
            std::cerr << "Return: " << git(wChr, mbChr.c_str(), 10) << '\n';
            /* Store wide char in cchar_t to be usable in ncurses functions */
            cchar_t cChr {};
            setcchar(&cChr, wChr, 0, 0, nullptr);
            std::wcerr << std::wstring{wChr}<< '\n';
            temp.emplace(std::wstring{wChr}, cChr);
        }
    }
    return temp;
}

///////////////////////////////////// BUTTON /////////////////////////////////////

Screen::Button::Button(WINDOW *win, int y, int x, int yLen, int xLen,
        std::function<void()> click, std::function<void(WINDOW *)> draw) :
    ptr{win},
    yTop{y}, yBtm{y + yLen}, xLeft{x}, xRight{x + xLen},
    click{click}, draw{draw}
{
    
}

/*==============================================================================*/

void Screen::Button::highlight(int attrs)
{
    wattron(ptr.get(), attrs);
    draw(ptr.get());
    wattroff(ptr.get(), attrs);
}

///////////////////////////////// BUTTON MANAGER /////////////////////////////////

// Screen::ButtonManager::ButtonManager(WINDOW *win, int y, int x,
//         size_t yLen, size_t xLen, size_t n) :
//     list{},
//     btn{0}
// {
//     initButtons(win, y, x, 0, 0, n);
// }

// void Screen::ButtonManager::initButtons(WINDOW *win, int y, int x,
//         size_t yLen, size_t xLen, size_t n)
// {

// }
