#include "TermiCube.h"


// int main() {

//     GameWindow game;

//     while(game.update() == 0);

//     return 0;
// }
  


// #include <iostream>
// #include <stdlib.h>
// #include <string>
// #include <cstring>

// // #include "atlbase.h"
// // #include "atlstr.h"
// // #include "comutil.h"

// using namespace std;
// // using namespace System;

// int main()
// {
//     // Create a string of wide characters, display it, and then
//     // use this string to create other types of strings.
//     wchar_t *orig = L"Test";
//     wcout << orig << L" (wchar_t *)" << endl;

//     // Convert the wchar_t string to a char* string. Record
//     // the length of the original string and add 1 to it to
//     // account for the terminating null character.
//     size_t origsize = wcslen(orig) + 1;
//     size_t convertedChars = 0;

//     // Use a multibyte string to append the type of string
//     // to the new string before displaying the result.
//     char strConcat[] = " (char *)";
//     size_t strConcatsize = (strlen( strConcat ) + 1)*2;

//     // Allocate two bytes in the multibyte output string for every wide
//     // character in the input string (including a wide character
//     // null). Because a multibyte character can be one or two bytes,
//     // you should allot two bytes for each character. Having extra
//     // space for the new string is not an error, but having
//     // insufficient space is a potential security problem.
//     const size_t newsize = origsize*2;
//     // The new string will contain a converted copy of the original
//     // string plus the type of string appended to it.
//     char *nstring = new char[newsize+strConcatsize];

//     // Put a copy of the converted string into nstring
//     wcstombs_s(&convertedChars, nstring, newsize, orig, _TRUNCATE);
//     // append the type of string to the new string.
//     // _mbscat_s((unsigned char*)nstring, newsize+strConcatsize, (unsigned char*)strConcat);
//     // Display the result.
//     cout << nstring << endl;

// }

#include <iostream>
#include <string>
#include <cstring>
// #include <clocale>
// #include <cstdlib>
 
int main()
{
    setlocale(LC_ALL, "");
    // UTF-8 narrow multibyte encoding
    const wchar_t wstr[] = L"▄  ▄ ▄▄▄ ▄   ▄   ▄▄▄▄  ▄▄  ▄   ▄ ▄▄▄";
    std::wstring str {wstr};

    char mbstr[100];
    std::wcstombs(mbstr, wstr, 100);
    std::cout << "multibyte string: " << mbstr << '\n';
    std::cout << wcslen(wstr) << '\n';
    std::cout << wcslen(str.c_str()) << '\n';
    std::cout << sizeof(wstr) << '\n';
    std::cout << wcstombs(nullptr, wstr, 0) << '\n';
    std::cout << str.length() << '\n';
    std::cout << str.size() << '\n';
}