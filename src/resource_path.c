#include "mstruct.h"
#include "mextern.h"
#include "resource_path.h"

#define RP_MAX_LINE 8192

typedef struct rp_alias {
    char *legacy_hex;
    char *normalized;
} rp_alias;

static rp_alias *g_aliases = 0;
static int g_alias_count = 0;
static int g_alias_loaded = 0;

#ifdef USE_RUST_RESOLVER
extern int mr_manifest_load(const char *manifest_path);
extern int mr_resolve_legacy_path(const char *legacy_path, char *out, unsigned long out_sz);
static int g_rust_manifest_loaded = 0;
#endif

static char *rp_strdup(const char *s)
{
    unsigned long n;
    char *p;

    if(!s)
        return 0;

    n = (unsigned long)strlen(s) + 1;
    p = (char *)malloc(n);
    if(!p)
        return 0;

    memcpy(p, s, n);
    return p;
}

static void rp_get_runtime_root(char *out, unsigned long out_sz)
{
    char *env_root;

    if(!out || out_sz == 0)
        return;

    env_root = getenv("MUHAN_HOME");
    if(env_root && env_root[0]) {
        snprintf(out, out_sz, "%s", env_root);
        return;
    }

    snprintf(out, out_sz, "%s", MUDHOME);
}

static int rp_path_exists(const char *path)
{
    if(!path || !path[0])
        return 0;
    return access(path, F_OK) == 0;
}

static int rp_starts_with(const char *s, const char *prefix)
{
    unsigned long n;

    if(!s || !prefix)
        return 0;

    n = (unsigned long)strlen(prefix);
    if(strlen(s) < n)
        return 0;

    return strncmp(s, prefix, n) == 0;
}

static void rp_bytes_to_hex(const unsigned char *in, char *out, unsigned long out_sz)
{
    static const char hex[] = "0123456789ABCDEF";
    unsigned long i, j, n;

    if(!in || !out || out_sz == 0)
        return;

    n = (unsigned long)strlen((const char *)in);
    j = 0;
    for(i = 0; i < n && (j + 2) < out_sz; i++) {
        out[j++] = hex[(in[i] >> 4) & 0x0f];
        out[j++] = hex[in[i] & 0x0f];
    }
    out[j] = 0;
}

static int rp_is_read_mode(const char *mode)
{
    if(!mode || !mode[0])
        return 1;

    if(strchr(mode, 'w') || strchr(mode, 'a') || strchr(mode, '+'))
        return 0;

    return 1;
}

static void rp_load_aliases(void)
{
    char root[512], alias_file[1024], line[RP_MAX_LINE];
    FILE *fp;

    if(g_alias_loaded)
        return;
    g_alias_loaded = 1;

    rp_get_runtime_root(root, sizeof(root));
    snprintf(alias_file, sizeof(alias_file), "%s/resources_manifest/path-alias.v1.tsv", root);

    fp = fopen(alias_file, "r");
    if(!fp)
        return;

    while(fgets(line, sizeof(line), fp)) {
        char *legacy_hex;
        char *legacy_cp949;
        char *normalized;
        char *sha;
        rp_alias *tmp;

        if(strncmp(line, "legacy_path_hex\t", 16) == 0)
            continue;

        legacy_hex = strtok(line, "\t\r\n");
        legacy_cp949 = strtok(0, "\t\r\n");
        normalized = strtok(0, "\t\r\n");
        sha = strtok(0, "\t\r\n");

        (void)legacy_cp949;
        (void)sha;

        if(!legacy_hex || !normalized)
            continue;

        tmp = (rp_alias *)realloc(g_aliases, sizeof(rp_alias) * (g_alias_count + 1));
        if(!tmp)
            continue;

        g_aliases = tmp;
        g_aliases[g_alias_count].legacy_hex = rp_strdup(legacy_hex);
        g_aliases[g_alias_count].normalized = rp_strdup(normalized);
        if(g_aliases[g_alias_count].legacy_hex && g_aliases[g_alias_count].normalized)
            g_alias_count++;
    }

    fclose(fp);
}

static const char *rp_find_alias(const char *legacy_hex)
{
    int i;

    if(!legacy_hex)
        return 0;

    rp_load_aliases();
    for(i = 0; i < g_alias_count; i++) {
        if(strcmp(g_aliases[i].legacy_hex, legacy_hex) == 0)
            return g_aliases[i].normalized;
    }

    return 0;
}

int resolve_legacy_path(const char *legacy_path, char *out, unsigned long out_sz)
{
    char root[512], adjusted[1024], hexbuf[2048], candidate[2048];
    const char *rel = 0;
    const char *alias;
    unsigned long root_len;

    if(!legacy_path || !out || out_sz == 0)
        return -1;

    out[0] = 0;

    if(rp_path_exists(legacy_path)) {
        snprintf(out, out_sz, "%s", legacy_path);
        return 0;
    }

    rp_get_runtime_root(root, sizeof(root));
    root_len = (unsigned long)strlen(root);

#ifdef USE_RUST_RESOLVER
    if(!g_rust_manifest_loaded) {
        char manifest_path[1024];
        snprintf(manifest_path, sizeof(manifest_path), "%s/resources_manifest/path-alias.v1.tsv", root);
        g_rust_manifest_loaded = (mr_manifest_load(manifest_path) == 0) ? 1 : -1;
    }

    if(g_rust_manifest_loaded == 1) {
        if(mr_resolve_legacy_path(legacy_path, candidate, sizeof(candidate)) == 0) {
            if(candidate[0] == '/') {
                snprintf(out, out_sz, "%s", candidate);
            } else {
                char rust_joined[2048];
                snprintf(rust_joined, sizeof(rust_joined), "%s/resources_utf8/%s", root, candidate);
                snprintf(out, out_sz, "%s", rust_joined);
            }
            return rp_path_exists(out) ? 0 : -1;
        }
    }
#endif

    adjusted[0] = 0;
    if(rp_starts_with(legacy_path, MUDHOME) && strcmp(root, MUDHOME) != 0) {
        snprintf(adjusted, sizeof(adjusted), "%s%s", root, legacy_path + strlen(MUDHOME));
        if(rp_path_exists(adjusted)) {
            snprintf(out, out_sz, "%s", adjusted);
            return 0;
        }
    }

    if(rp_starts_with(legacy_path, MUDHOME "/"))
        rel = legacy_path + strlen(MUDHOME) + 1;
    else if(root_len > 0 && rp_starts_with(legacy_path, root) && legacy_path[root_len] == '/')
        rel = legacy_path + root_len + 1;
    else if(legacy_path[0] != '/')
        rel = legacy_path;

    if(rel && rel[0]) {
        rp_bytes_to_hex((const unsigned char *)rel, hexbuf, sizeof(hexbuf));
        alias = rp_find_alias(hexbuf);

        if(alias && alias[0]) {
            snprintf(candidate, sizeof(candidate), "%s/resources_utf8/%s", root, alias);
            if(rp_path_exists(candidate)) {
                snprintf(out, out_sz, "%s", candidate);
                return 0;
            }
        }

        snprintf(candidate, sizeof(candidate), "%s/resources_utf8/%s", root, rel);
        if(rp_path_exists(candidate)) {
            snprintf(out, out_sz, "%s", candidate);
            return 0;
        }
    }

    if(adjusted[0]) {
        snprintf(out, out_sz, "%s", adjusted);
        return rp_path_exists(adjusted) ? 0 : -1;
    }

    snprintf(out, out_sz, "%s", legacy_path);
    return rp_path_exists(legacy_path) ? 0 : -1;
}

FILE *rp_fopen(const char *path, const char *mode)
{
    char resolved[2048];

    if(rp_is_read_mode(mode)) {
        if(resolve_legacy_path(path, resolved, sizeof(resolved)) == 0)
            return fopen(resolved, mode);
    } else {
        char root[512], adjusted[2048];
        rp_get_runtime_root(root, sizeof(root));
        if(rp_starts_with(path, MUDHOME) && strcmp(root, MUDHOME) != 0) {
            snprintf(adjusted, sizeof(adjusted), "%s%s", root, path + strlen(MUDHOME));
            return fopen(adjusted, mode);
        }
    }

    return fopen(path, mode);
}

int rp_open(const char *path, int flags, int mode)
{
    char resolved[2048];
    int read_only;

    read_only = ((flags & O_ACCMODE) == O_RDONLY) && !(flags & O_CREAT);

    if(read_only) {
        if(resolve_legacy_path(path, resolved, sizeof(resolved)) == 0)
            return open(resolved, flags, mode);
    } else {
        char root[512], adjusted[2048];
        rp_get_runtime_root(root, sizeof(root));
        if(rp_starts_with(path, MUDHOME) && strcmp(root, MUDHOME) != 0) {
            snprintf(adjusted, sizeof(adjusted), "%s%s", root, path + strlen(MUDHOME));
            return open(adjusted, flags, mode);
        }
    }

    return open(path, flags, mode);
}

int rp_stat(const char *path, struct stat *st)
{
    char resolved[2048];

    if(resolve_legacy_path(path, resolved, sizeof(resolved)) == 0)
        return stat(resolved, st);

    return stat(path, st);
}
