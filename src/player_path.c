#include "mstruct.h"
#include "mextern.h"
#include "player_path.h"
#include "utf8_text.h"

#include <string.h>
#include <sys/stat.h>

static unsigned int rol32(unsigned int v, int n)
{
    return (v << n) | (v >> (32 - n));
}

static unsigned int load_u32_be(const unsigned char *p)
{
    return ((unsigned int)p[0] << 24) |
           ((unsigned int)p[1] << 16) |
           ((unsigned int)p[2] << 8) |
           (unsigned int)p[3];
}

static void store_u32_be(unsigned int v, unsigned char *p)
{
    p[0] = (unsigned char)((v >> 24) & 0xff);
    p[1] = (unsigned char)((v >> 16) & 0xff);
    p[2] = (unsigned char)((v >> 8) & 0xff);
    p[3] = (unsigned char)(v & 0xff);
}

static void sha1_digest(const unsigned char *data, unsigned long len, unsigned char out[20])
{
    unsigned int h0, h1, h2, h3, h4;
    unsigned char block[128];
    unsigned long total, off;
    unsigned int w[80];
    int i;

    h0 = 0x67452301U;
    h1 = 0xEFCDAB89U;
    h2 = 0x98BADCFEU;
    h3 = 0x10325476U;
    h4 = 0xC3D2E1F0U;

    total = (len + 9 <= 64) ? 64 : 128;
    memset(block, 0, sizeof(block));
    if(len > 0)
        memcpy(block, data, len);
    block[len] = 0x80;

    {
        unsigned int bit_hi;
        unsigned int bit_lo;
        unsigned long idx;
        idx = total - 8;
        bit_hi = (unsigned int)(len >> 29);
        bit_lo = (unsigned int)(len << 3);
        store_u32_be(bit_hi, &block[idx]);
        store_u32_be(bit_lo, &block[idx + 4]);
    }

    for(off = 0; off < total; off += 64) {
        unsigned int a, b, c, d, e, f, k, t, temp;

        for(i = 0; i < 16; i++)
            w[i] = load_u32_be(&block[off + (i * 4)]);
        for(i = 16; i < 80; i++)
            w[i] = rol32(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);

        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;

        for(i = 0; i < 80; i++) {
            if(i < 20) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999U;
            } else if(i < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1U;
            } else if(i < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDCU;
            } else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6U;
            }

            t = (rol32(a, 5) + f + e + k + w[i]) & 0xffffffffU;
            e = d;
            d = c;
            c = rol32(b, 30);
            b = a;
            a = t;
        }

        h0 = (h0 + a) & 0xffffffffU;
        h1 = (h1 + b) & 0xffffffffU;
        h2 = (h2 + c) & 0xffffffffU;
        h3 = (h3 + d) & 0xffffffffU;
        h4 = (h4 + e) & 0xffffffffU;
    }

    store_u32_be(h0, &out[0]);
    store_u32_be(h1, &out[4]);
    store_u32_be(h2, &out[8]);
    store_u32_be(h3, &out[12]);
    store_u32_be(h4, &out[16]);
}

static void player_shard(const char *name, char out[3])
{
    static const char hex[] = "0123456789abcdef";
    unsigned char digest[20];
    unsigned char b;

    sha1_digest((const unsigned char *)name, (unsigned long)strlen(name), digest);
    b = digest[0];
    out[0] = hex[(b >> 4) & 0x0f];
    out[1] = hex[b & 0x0f];
    out[2] = 0;
}

int player_path_from_name(const char *name, char *out, unsigned long out_sz)
{
    char shard[3];

    if(!name || !name[0] || !out || out_sz == 0)
        return -1;

    player_shard(name, shard);
    snprintf(out, out_sz, "%s/%s/%s", PLAYERPATH, shard, name);
    return 0;
}

int player_path_ensure_dir(const char *name)
{
    char shard[3], dir[256];
    struct stat st;

    if(!name || !name[0])
        return -1;

    player_shard(name, shard);
    snprintf(dir, sizeof(dir), "%s/%s", PLAYERPATH, shard);
    if(stat(dir, &st) == 0) {
        if(S_ISDIR(st.st_mode))
            return 0;
        return -1;
    }

    if(mkdir(dir, 0770) == 0)
        return 0;

    if(stat(dir, &st) == 0 && S_ISDIR(st.st_mode))
        return 0;

    return -1;
}

int player_name_is_valid(const unsigned char *name, unsigned long min_cp, unsigned long max_cp)
{
    unsigned long i, n, cp_len;

    if(!name || !name[0])
        return 0;

    n = (unsigned long)strlen((const char *)name);
    if(!utf8_validate(name, n))
        return 0;

    cp_len = utf8_codepoint_len(name);
    if(cp_len < min_cp || cp_len > max_cp)
        return 0;

    if(strcmp((const char *)name, ".") == 0 || strcmp((const char *)name, "..") == 0)
        return 0;

    for(i = 0; i < n; i++) {
        if(name[i] < 32 || name[i] == 127)
            return 0;
        if(name[i] == '/' || name[i] == '\\' || name[i] == ':')
            return 0;
    }

    return 1;
}
