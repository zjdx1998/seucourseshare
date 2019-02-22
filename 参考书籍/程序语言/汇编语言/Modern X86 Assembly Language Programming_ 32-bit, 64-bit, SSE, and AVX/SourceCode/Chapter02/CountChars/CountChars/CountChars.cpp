#include "stdafx.h"

extern "C" int CountChars_(wchar_t* s, wchar_t c);

int _tmain(int argc, _TCHAR* argv[])
{
    wchar_t c;
    wchar_t* s;

    s = L"Four score and seven seconds ago, ...";
    wprintf(L"\nTest string: %s\n", s);
    c = L's';
    wprintf(L"  SearchChar: %c Count: %d\n", c, CountChars_(s, c));
    c = L'F';
    wprintf(L"  SearchChar: %c Count: %d\n", c, CountChars_(s, c));
    c = L'o';
    wprintf(L"  SearchChar: %c Count: %d\n", c, CountChars_(s, c));
    c = L'z';
    wprintf(L"  SearchChar: %c Count: %d\n", c, CountChars_(s, c));

    s = L"Red Green Blue Cyan Magenta Yellow";
    wprintf(L"\nTest string: %s\n", s);
    c = L'e';
    wprintf(L"  SearchChar: %c Count: %d\n", c, CountChars_(s, c));
    c = L'w';
    wprintf(L"  SearchChar: %c Count: %d\n", c, CountChars_(s, c));
    c = L'Q';
    wprintf(L"  SearchChar: %c Count: %d\n", c, CountChars_(s, c));
    c = L'l';
    wprintf(L"  SearchChar: %c Count: %d\n", c, CountChars_(s, c));

    return 0;
}
