#ifndef UTF8_TEXT_H
#define UTF8_TEXT_H

int utf8_validate(const unsigned char *s, unsigned long n);
unsigned long utf8_codepoint_len(const unsigned char *s);
int utf8_ends_with(const unsigned char *s, const unsigned char *suffix);
int utf8_prev_char_start(const unsigned char *s, int len);
int utf8_has_jongseong_last(const unsigned char *s);

#endif
