#include "TCWindow.h"


int main() {

    TCWindow game;

    while (!game.shouldClose())
        game.update();

    return 0;
}

// #define _XOPEN_SOURCE_EXTENDED 1

// #include <iostream>
// #include <fstream>
// #include <unordered_map>
// #include <string>
// #include <panel.h>

// void func()
// {
//     std::ifstream file {"resource/general/Unicode"};

//     if (!file)
//         std::cerr << "File could not be opened\n";

//     std::unordered_map<std::wstring, cchar_t> temp;
//     std::string mbChr;
//     wchar_t wChr[1000];
//     /* Store file content into multibyte strings */
//     while (file >> mbChr) {
//         /* Move to next line if comment symbol "//"" is found */
//         if (mbChr.compare("//") == 0)
//             std::getline(file, mbChr);
//         else {
//             std::cerr << mbChr << '\n';
//             /* Convert multibyte string to wide char string */
//             std::cerr << "Return: " << mbstowcs(wChr, mbChr.c_str(), 1000) << '\n';
//             /* Store wide char in cchar_t to be usable in ncurses functions */
//             cchar_t cChr {};
//             setcchar(&cChr, wChr, 0, 0, nullptr);
//             std::wcerr << wChr;
//             std::cerr << '\n';
//             temp.emplace(std::wstring{wChr}, cChr);
//             std::cerr << temp.size() << '\n';
//         }
//     }
// }

// int main()
// {
//     setlocale(LC_ALL, "");
//     func();
// }

// #include <panel.h>
// #include <iostream>

// int main()
// {
//     initscr();
//     cbreak();
//     noecho();

//     int key;
//     auto *ptr {newwin(10, 10, 10, 10)};

//     while ((key = wgetch(ptr)) != 'q') {
//         // nodelay(ptr, TRUE);

//         // if (wgetch(ptr) == 91) {
//         //     if (wgetch(ptr) == 67) {
//         //         std::cerr << "KEY_RIGHT\n";
//         //         nodelay(ptr, FALSE);
//         //         continue;
//         //     }
//         // }

//         // if ((key = wgetch(ptr)) == ERR)
//         //     key = 27;
//         // else if (key == 91) {
//         //     if (65 <= (key = wgetch(ptr)) && key <= 68) /* Arrow Keys */
//         //         std::cerr << "Yay\n";
//         // }
//         std::cerr << "Key: " << key << '\n';
//         // nodelay(ptr, FALSE);
//     }


//     endwin();
// }
