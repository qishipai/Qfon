/* 2022-10-13 by 云中龙++ : v1.6 */

#pragma once

typedef unsigned int   ui32;
typedef unsigned short ui16;
typedef unsigned char  byte;


typedef struct {
    ui32 ch_ct;
    ui16 ch_cb;
    ui16 ch_ro;
    byte *data; 
} Qfon_ctx;


static byte fonLoad(Qfon_ctx *c,
  char const *fn, ui16 cb, ui16 ro)
{
    FILE *fon = fopen(fn, "rb");

    if (!fon)
    {
        puts("fonLoad():"
         "文件不存在或无法访问！");
        return 1;
    }

    fseeko(fon, 0ll, SEEK_END);
    off_t sz = ftello(fon);
    fseeko(fon, 0ll, SEEK_SET);

    c->data = malloc(sz);
    c->ch_ct = sz / (cb * ro);
    fread(c->data, sz, 1, fon);
    c->ch_cb = cb, c->ch_ro = ro;
    return 0;
}

static void fonFree(Qfon_ctx *c)
{
    free(c->data);
}

static byte fonDrawPix(Qfon_ctx *c,
  byte *mat, ui16 mw, char const *str)
{
    ui16 sz = c->ch_cb * c->ch_ro;
    ui16 cw = c->ch_cb * 8;
    ui32 dpt = mw - cw, id;
    byte c0, c1;

    for (ui32 of = 0; *str; of += cw)
    {
        if ((c0 = *str++) < 161)
        {
            if (c0 < 32 || c0 > 126)
            {
                puts("fonDrawPix():"
                 "ASCII映射失败！");
                return 1;
            }

            id = c0 > 32? (c0 < 126?
              c0 + 155 : 10) : 0;
        }
        else
        {
            if ((c1 = *str++) < 161)
            {
                puts("fonDrawPix():"
                 "不支持的字符编码！");
                return 1;
            }

            c0 -= 161;
            id = 94 * c0 + c1 - 161;
        }

        if (id >= c->ch_ct)
        {
            puts("fonDrawPix():"
             "字符串内包含缺失汉字！");
            return 1;
        }

        byte *p = c->data + id * sz;
        byte *pmt = mat + of;
        ui16 chro = c->ch_ro;

        for ( ; chro--; pmt += dpt)
        {
            ui16 ccb = c->ch_cb;

            for ( ; ccb--; pmt += 8)
            {
                byte b = *p++, i = 8;

                for ( ; i; b >>= 1)
                {
                    pmt[--i] = b & 1;
                }
            }
        }
    }

    return 0;
}

static byte fonAscIIArt(Qfon_ctx *c,
           ui32 cs, char const *u,
     char const *v, char const *s)
{
    ui16 r = c->ch_ro;
    ui32 w = c->ch_cb * cs * 8; 
    byte *b = calloc(w, r), *p;

    if (fonDrawPix(c, b, w, s))
    {
        return 1;
    }

    for (p = b; r--; puts(""))
    {
        for (ui32 i = w; i--; )
        {
            fputs(*p++? v : u,
                       stdout);
        }
    }

    free(b);
    return (fflush(stdout), 0);
}
