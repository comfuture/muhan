#ifndef PLAYER_PATH_H
#define PLAYER_PATH_H

int player_path_from_name(const char *name, char *out, unsigned long out_sz);
int player_path_ensure_dir(const char *name);
int player_name_is_valid(const unsigned char *name, unsigned long min_cp, unsigned long max_cp);

#endif
