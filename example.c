#include <stdio.h>
#include <stdlib.h>
#include "Qfon.h"

int main()
{
    Qfon_ctx fon;
    fonLoad(&fon, "chs16.fon", 2, 16);
    fonAscIIArt(&fon, 5, "  ", "#;", "������++");
    fonFree(&fon);
    return 0;
}
