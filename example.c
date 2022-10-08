#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Qfon.h"

#define MAXW 4  // ���������

byte scr[16 * 16 * MAXW];

int main()
{
    Qfon_ctx fon;

    /* �������� */
    if (fonLoad(&fon, "chs16.fon", 2, 16))
    {
        return 1;
    }

    fonShowChAscii(&fon, scr, MAXW,
        "  " /* 0��Ӧ���ִ� */,
        "##" /* 1��Ӧ���ִ� */, 
        "��̳�" /*����ʾ���ִ�������<=MAXW��*/ );

    fonFree(&fon);
    return 0;
}
