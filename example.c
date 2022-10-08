#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Qfon.h"

#define MAXW 4  // 最大单行字数

byte scr[16 * 16 * MAXW];

int main()
{
    Qfon_ctx fon;

    /* 加载字体 */
    if (fonLoad(&fon, "chs16.fon", 2, 16))
    {
        return 1;
    }

    fonShowChAscii(&fon, scr, MAXW,
        "  " /* 0对应的字串 */,
        "##" /* 1对应的字串 */, 
        "冲刺冲" /*想显示的字串（字数<=MAXW）*/ );

    fonFree(&fon);
    return 0;
}
