#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Qfon.h"

#define UTF8_ONLY

#ifdef UTF8_ONLY
#include <iconv.h>
#endif

#define MAXW 4 // 最大单行字数

byte scr[16 * 16 * MAXW];

int main()
{
    Qfon_ctx fon;

    /* 加载字体 */
    if (fonLoad(&fon, "chs16.fon", 2, 16))
    {
        return 1;
    }

    char s[] = "我来！", g[35];

#ifdef UTF8_ONLY
    iconv_t ic;
    char *bp = g, *sp = s;
    size_t inl = sizeof(s);
    size_t oul = sizeof(g);
    ic = iconv_open("GB2312", "UTF-8");
    iconv(ic, &sp, &inl, &bp, &oul);
    iconv_close(ic);
#else
    strcpy(g, s);
#endif

     fonShowChAscii(&fon, scr, MAXW,
        "  " /* 0对应的字串 */,
        "#:" /* 1对应的字串 */, 
        g /*想显示的字串（字数<=MAXW）*/ );

    fonFree(&fon);
    return 0;
}
