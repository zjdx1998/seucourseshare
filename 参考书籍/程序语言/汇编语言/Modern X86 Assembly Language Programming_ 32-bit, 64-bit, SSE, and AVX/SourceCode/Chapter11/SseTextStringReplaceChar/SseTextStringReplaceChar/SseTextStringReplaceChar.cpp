#include "stdafx.h"
#include <string.h>
#include <malloc.h>

extern "C" int SseTextStringReplaceChar_(char* s, char old_char, char new_char);

const char* TestStrings[] =
{
    "*Red*Green*Blue*",
    "Cyan*Magenta Yellow*Black Tan",
    "White*Pink Brown Purple*Gray Orange*",
    "Beige Silver Indigo Fuchsia Maroon",
    "***************",
    "*****+*****+*****+*****+*****",
    ""
};

const char OldChar = '*';
const char NewChar = '#';
const int OffsetMin = 4096 - 40;
const int OffsetMax = 4096 + 40;
const int NumTestStrings = sizeof(TestStrings) / sizeof (char*);
const unsigned int CheckNum = 0x12345678;

int SseTextStringReplaceCharCpp(char* s, char old_char, char new_char)
{
    char c;
    int n = 0;

    while ((c = *s) != '\0')
    {
        if (c == OldChar)
        {
            *s = NewChar;
            n++;
        }

        s++;

    }

    return n;
}

void SseTextStringReplaceChar(void)
{
    const int buff_size = 8192;
    const int page_size = 4096;
    char* buff1 = (char*)_aligned_malloc(buff_size, page_size);
    char* buff2 = (char*)_aligned_malloc(buff_size, page_size);

    printf("\nResults for SseTextStringReplaceChars()\n");
    printf("OldChar = '%c'  NewChar = '%c'\n", OldChar, NewChar);

    for (int i = 0; i < NumTestStrings; i++)
    {
        const char* s = TestStrings[i];
        int s_len = strlen(s);

        for (int offset = OffsetMin; offset <= OffsetMax; offset++)
        {
            bool print = (offset == OffsetMin) ? true : false;
            char* s1 = buff1 + offset;
            char* s2 = buff2 + offset;
            int size = buff_size - offset;
            int n1 = -1, n2 = -1;

            strcpy_s(s1, size, s);
            *(s1 + s_len + 1) = OldChar;
            *((unsigned int*)(s1 + s_len + 2)) = CheckNum;

            strcpy_s(s2, size, s);
            *(s2 + s_len + 1) = OldChar;
            *((unsigned int*)(s2 + s_len + 2)) = CheckNum;

            if (print)
                printf("\ns1 before replace: \"%s\"\n", s1);
            n1 = SseTextStringReplaceCharCpp(s1, OldChar, NewChar);
            if (print)
                printf("s1 after replace:  \"%s\"\n", s1);

            if (print)
                printf("\ns2 before replace: \"%s\"\n", s2);
            n2 = SseTextStringReplaceChar_(s2, OldChar, NewChar);
            if (print)
                printf("s2 after replace:  \"%s\"\n", s2);

            if (strcmp(s1, s1) != 0)
                printf("Error - string compare failed\n");
            if (n1 != n2)
                printf("Error - character count compare failed\n");

            if (*(s1 + s_len + 1) != OldChar)
                printf("Error - buff1 OldChar overwrite\n");
            if (*(s2 + s_len + 1) != OldChar)
                printf("Error - buff2 OldChar overwrite\n");

            if (*((unsigned int*)(s1 + s_len + 2)) != CheckNum)
                printf("Error - buff1 CheckNum overwrite\n");
            if (*((unsigned int*)(s2 + s_len + 2)) != CheckNum)
                printf("Error - buff2 CheckNum overwrite\n");
        }
    }

    _aligned_free(buff1);
    _aligned_free(buff2);
}

int _tmain(int argc, _TCHAR* argv[])
{
    SseTextStringReplaceChar();
    return 0;
}
