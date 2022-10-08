/* 2022-10-07 by 云中龙++ : v1.5 */

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
    byte c0, c1;  ui32 id;

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
        ui16 ro = c->ch_ro, cb;
        byte *u = mat + of, *d;

        for ( ; ro--; u += mw)
        {
            cb = c->ch_cb, d = u;

            while (cb--)
            {
                byte b = *p++, i = 128;

                for ( ; i; i >>= 1)
                {
                    *d++ = (b & i) > 0;
                }
            }
        }
    }

    return 0;
}

static byte fonShowChAscii(Qfon_ctx *c,
     byte *buf, ui32 bw, char const *u,
     char const *v, char const *str)
{
    ui16 m = strlen(u), n = strlen(v);
    ui32 matw = c->ch_cb * bw * 8;
    ui16 r = 0;

    if (fonDrawPix(c, buf, matw, str))
    {
        return 1;
    }

    while (1)
    {
        for (ui32 w = 0; w < matw; ++w)
        {
            if (*buf++)
            {
                fwrite(v, n, 1, stdout);
            }
            else
            {
                fwrite(u, m, 1, stdout);
            }
        }

        putchar('\n');

        if (++r == c->ch_ro)
        {
            return (fflush(stdout), 0);
        }
    }
}
