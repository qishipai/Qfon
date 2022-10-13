#include <stdio.h>
#include <stdlib.h>
#include "Qfon_em.h"

int main()
{
    Qfon_ctx fon;
    fonLoad_em(&fon);
    fonAscIIArt(&fon, 5, "  ", "#;", "ÔÆÖÐÁú++");
    fonFree(&fon);
    return 0;
}
