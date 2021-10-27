#include "TermiCube.h"


int main() {

    GameWindow game;

    while(game.update() == 0);

    return 0;
}
  


// #include <iostream>
// #include <string>
// #include <cstring>
// #include <bitset>
// #include <fstream>
 
// int main()
// {
//     setlocale(LC_ALL, "");
//     // UTF-8 narrow multibyte encoding
//     std::string path {"resource/mainmenu/NewGameBtn.txt"};
//     std::ifstream file {path};
//     std::string line;

//     if (!file)
//         std::cerr << "File could not be opened: " << path << '\n';

//     while (std::getline(file, line)) {
//         std::cout << line << '\n';
//         size_t space {0}, count {0};

//         for (size_t i {0}; i < line.length(); i++) {
//             if (line[i] == ' ')
//                 space++;
//             /* In Windows a newline is represented in the CR + LF format: "\r\n" */
//             else if (line[i] != '\r') {
//                 std::bitset<8> byte {static_cast<unsigned long long>(line[i])};
//                 size_t bits {0};

//                 for (size_t j {0}; j < 8 && byte.test(7 - j); j++, bits++);

//                 if (bits == 3 || bits == 4) {
//                     i += bits - 1;
//                 } else if (bits == 2) {
//                     std::bitset<8> nextByte {static_cast<unsigned long long>(line[++i])};

//                     if (!nextByte.test(7) || nextByte.test(6)) {
//                         std::cerr << "No continuation byte found\n";
//                         exit(1);
//                     }
//                 }
//                 count++;
//             }
//         }
//         std::cout << "\nSpaces : " << space << '\n';
//         std::cout << "\nUnicode Chars : " << count << "\n\n";
//     }
// }