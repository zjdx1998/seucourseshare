#include "stdafx.h"
#include <malloc.h>
#include <string.h>

extern "C" int SseTextStringCalcLength_(const char* s);

const char * TestStrings[] =
{
        "0123456",                                  // Length = 7
        "0123456789abcde",                          // Length = 15
        "0123456789abcdef",                         // Length = 16
        "0123456789abcdefg",                        // Length = 17
        "0123456789abcdefghijklmnopqrstu",          // Length = 31
        "0123456789abcdefghijklmnopqrstuv",         // Length = 32
        "0123456789abcdefghijklmnopqrstuvw",        // Length = 33
        "0123456789abcdefghijklmnopqrstuvwxyz",     // Length = 36
        "",                                         // Length = 0
};

const int OffsetMin = 4096 - 40;
const int OffsetMax = 4096 + 40;
const int NumTestStrings = sizeof(TestStrings) / sizeof(char*);

void SseTextStringCalcLength(void)
{
    const int buff_size = 8192;
    const int page_size = 4096;
    char* buff = (char*)_aligned_malloc(buff_size, page_size);

    printf("\nResults for SseTextStringCalcLength()\n");

    for (int i = 0; i < NumTestStrings; i++)
    {
        bool error = false;
        const char* ts = TestStrings[i];

        printf("Test string: \"%s\"\n", ts);

        for (int offset = OffsetMin; offset <= OffsetMax; offset++)
        {
            char* s2 = buff + offset;

            memset(buff, 0x55, buff_size);
            strcpy_s(s2, buff_size - offset, ts);

            int len1 = strlen(s2);
            int len2 = SseTextStringCalcLength_(s2);

            if ((len1 != len2) && !error)
            {
                error = true;
                printf("  String length compare failed!\n");
                printf("  buff: 0x%p  offset: %5d  s2: 0x%p", buff, offset, s2);
                printf("  len1: %5d  len2: %5d\n",len1, len2);
            }
        }

        if (!error)
            printf("  No errors detected\n");
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    SseTextStringCalcLength();
    return 0;
}
