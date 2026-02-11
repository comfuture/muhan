#include "utf8_text.h"

#include <string.h>

static int utf8_decode_one(const unsigned char *s, unsigned long n, unsigned long *consumed, unsigned long *cp)
{
    unsigned long c0, c1, c2, c3;

    if(!s || n == 0 || !consumed || !cp)
        return 0;

    c0 = (unsigned long)s[0];
    if(c0 < 0x80UL) {
        *consumed = 1;
        *cp = c0;
        return 1;
    }

    if((c0 & 0xE0UL) == 0xC0UL) {
        if(n < 2)
            return 0;
        c1 = (unsigned long)s[1];
        if((c1 & 0xC0UL) != 0x80UL)
            return 0;
        *cp = ((c0 & 0x1FUL) << 6) | (c1 & 0x3FUL);
        if(*cp < 0x80UL)
            return 0;
        *consumed = 2;
        return 1;
    }

    if((c0 & 0xF0UL) == 0xE0UL) {
        if(n < 3)
            return 0;
        c1 = (unsigned long)s[1];
        c2 = (unsigned long)s[2];
        if((c1 & 0xC0UL) != 0x80UL || (c2 & 0xC0UL) != 0x80UL)
            return 0;
        *cp = ((c0 & 0x0FUL) << 12) | ((c1 & 0x3FUL) << 6) | (c2 & 0x3FUL);
        if(*cp < 0x800UL)
            return 0;
        if(*cp >= 0xD800UL && *cp <= 0xDFFFUL)
            return 0;
        *consumed = 3;
        return 1;
    }

    if((c0 & 0xF8UL) == 0xF0UL) {
        if(n < 4)
            return 0;
        c1 = (unsigned long)s[1];
        c2 = (unsigned long)s[2];
        c3 = (unsigned long)s[3];
        if((c1 & 0xC0UL) != 0x80UL || (c2 & 0xC0UL) != 0x80UL || (c3 & 0xC0UL) != 0x80UL)
            return 0;
        *cp = ((c0 & 0x07UL) << 18) | ((c1 & 0x3FUL) << 12) | ((c2 & 0x3FUL) << 6) | (c3 & 0x3FUL);
        if(*cp < 0x10000UL || *cp > 0x10FFFFUL)
            return 0;
        *consumed = 4;
        return 1;
    }

    return 0;
}

int utf8_validate(const unsigned char *s, unsigned long n)
{
    unsigned long i, c, cp;

    if(!s)
        return 0;

    i = 0;
    while(i < n) {
        if(!utf8_decode_one(s + i, n - i, &c, &cp))
            return 0;
        i += c;
    }

    return 1;
}

unsigned long utf8_codepoint_len(const unsigned char *s)
{
    unsigned long n, i, c, cp, count;

    if(!s)
        return 0;

    n = (unsigned long)strlen((const char *)s);
    i = 0;
    count = 0;
    while(i < n) {
        if(!utf8_decode_one(s + i, n - i, &c, &cp))
            return count;
        count++;
        i += c;
    }

    return count;
}

int utf8_ends_with(const unsigned char *s, const unsigned char *suffix)
{
    unsigned long n1, n2;

    if(!s || !suffix)
        return 0;

    n1 = (unsigned long)strlen((const char *)s);
    n2 = (unsigned long)strlen((const char *)suffix);
    if(n2 > n1)
        return 0;

    return memcmp(s + (n1 - n2), suffix, n2) == 0;
}

int utf8_prev_char_start(const unsigned char *s, int len)
{
    int i;
    unsigned long c, cp;

    if(!s || len <= 0)
        return 0;

    i = len - 1;
    while(i > 0 && (s[i] & 0xC0) == 0x80)
        i--;

    if(utf8_decode_one(s + i, (unsigned long)(len - i), &c, &cp) && (i + (int)c) == len)
        return i;

    return len - 1;
}

int utf8_has_jongseong_last(const unsigned char *s)
{
    unsigned long n, i, c, cp, last;

    if(!s)
        return 0;

    n = (unsigned long)strlen((const char *)s);
    i = 0;
    last = 0;
    while(i < n) {
        if(!utf8_decode_one(s + i, n - i, &c, &cp))
            return 0;
        last = cp;
        i += c;
    }

    if(last < 0xAC00UL || last > 0xD7A3UL)
        return 0;

    return (((last - 0xAC00UL) % 28UL) != 0UL) ? 1 : 0;
}
