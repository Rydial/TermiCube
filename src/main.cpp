#include "TermiCube.h"


int main() {

    GameWindow game;

    while(game.update() == 0);

    return 0;
}
  

// #include <iostream>
// #include <fstream>
// #include <string>
// #include <cstring>

// void parseTxt(std::vector<std::string> &vec, std::string path) {
//     std::wifstream file {path};

//     if (!file)
//         std::cerr << "File could not be opened: " << path << '\n';
    
//     std::string line;
//     // constexpr size_t arrSize {80 * sizeof(wchar_t) * 2};
    
//     while (std::getline(file, line)) {
//         // std::wstring_view lineV {line};
//         // toString
//         // std::wcerr << line << '\n';
//         // // size_t
//         // char arr[arrSize];
//         // wcstombs(arr, line.c_str(), arrSize);
//         // std::cout << strlen(arr) << '\n';
//         vec.emplace_back(line);
//     }
// }

// int main()
// {
//     setlocale(LC_ALL, "");
//     std::vector<std::string> vec;
//     parseTxt(vec, "resource/mainmenu/Title.txt");
    
//     for (auto i : vec)
//         std::cout << i << '\n';

//     std::vector<std::string> vec2;
//     parseTxt(vec2, "resource/mainmenu/LoadGameBtn.txt");
    
//     for (auto i : vec2)
//         std::cout << i << '\n';

//     std::vector<std::string> vec3;
//     parseTxt(vec3, "resource/mainmenu/NewGameBtn.txt");
    
//     for (auto i : vec3)
//         std::cout << i << '\n';
// }