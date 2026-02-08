#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "resource_path.h"

static int hexval(int c)
{
    if(c >= '0' && c <= '9') return c - '0';
    if(c >= 'a' && c <= 'f') return c - 'a' + 10;
    if(c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

int main(void)
{
    char line[8192];

    while(fgets(line, sizeof(line), stdin)) {
        unsigned char legacy[4096];
        char out[4096];
        char *p;
        int len, i, rc;

        p = line;
        while(*p && *p != '\n' && *p != '\r') p++;
        *p = 0;

        len = (int)strlen(line);
        if(len == 0) {
            continue;
        }

        if(len % 2 != 0 || len / 2 >= (int)sizeof(legacy)) {
            printf("%s\t-9\t\n", line);
            continue;
        }

        for(i = 0; i < len / 2; i++) {
            int hi = hexval((unsigned char)line[i * 2]);
            int lo = hexval((unsigned char)line[i * 2 + 1]);
            if(hi < 0 || lo < 0) {
                printf("%s\t-8\t\n", line);
                fflush(stdout);
                goto next_line;
            }
            legacy[i] = (unsigned char)((hi << 4) | lo);
        }
        legacy[len / 2] = 0;

        out[0] = 0;
        rc = resolve_legacy_path((const char *)legacy, out, sizeof(out));
        printf("%s\t%d\t%s\n", line, rc, (rc == 0) ? out : "");
        fflush(stdout);
next_line:
        ;
    }

    return 0;
}
