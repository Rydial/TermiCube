#define NCURSES_WIDECHAR 1

#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <unordered_map>
#include <panel.h>
#include "Utilites.h"

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
