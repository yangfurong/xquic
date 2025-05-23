/**
 * @copyright Copyright (c) 2022, Alibaba Group Holding Limited
 */

#ifndef _XQC_STR_H_INCLUDED_
#define _XQC_STR_H_INCLUDED_

#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <time.h>
#include <stddef.h>
#include <sys/types.h>

#include <include/xquic/xquic_typedef.h>
#include "src/common/xqc_config.h"

#ifndef  XQC_SYS_WINDOWS
#include <sys/resource.h>
#endif

typedef struct xqc_str_s {
    size_t          len;
    unsigned char  *data;
} xqc_str_t;

#define xqc_string(str)         { sizeof(str) - 1, (unsigned char *) str }
#define xqc_null_string         { 0, NULL }
#define xqc_str_set(str, text)  (str)->len = sizeof(text) - 1; (str)->data = (unsigned char *) text
#define xqc_str_null(str)       (str)->len = 0; (str)->data = NULL

#define xqc_str_equal(s1, s2)   ((s1).len == (s2).len && memcmp((s1).data, (s2).data, (s1).len) == 0)

#define xqc_tolower(c)          (unsigned char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define xqc_toupper(c)          (unsigned char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)


#define xqc_memzero(buf, n)     (void) memset(buf, 0, n)
#define xqc_memset(buf, c, n)   (void) memset(buf, c, n)

#define xqc_memcpy(dst, src, n) (void) memcpy(dst, src, n)
#define xqc_cpymem(dst, src, n) (((unsigned char *) memcpy(dst, src, n)) + (n))

#define xqc_memcmp(s1, s2, n)   memcmp((const char *) s1, (const char *) s2, n)

#define xqc_lengthof(x)         (sizeof(x) - 1)


unsigned char *xqc_hex_dump(unsigned char *dst, const unsigned char *src, size_t len);
unsigned char *xqc_vsprintf(unsigned char *buf, unsigned char *last, const char *fmt, va_list args);
unsigned char *xqc_sprintf_num(unsigned char *buf, unsigned char *last,
    uint64_t ui64, unsigned char zero, uintptr_t hexadecimal, uintptr_t width);

static inline int
xqc_memcpy_with_cap(void *dst, size_t cap, const void *src, size_t n)
{
    if (n == 0) {
        return XQC_OK;
    }

    if (n <= cap) {
        xqc_memcpy(dst, src, n);
        return XQC_OK;
    }

    return -XQC_ENOBUF;
}

static inline void
xqc_str_tolower(unsigned char *dst, unsigned char *src, size_t n)
{
    while (n) {
        *dst = xqc_tolower(*src);
        dst++;
        src++;
        n--;
    }
}

static inline unsigned char * 
xqc_sprintf(unsigned char *buf, unsigned char *last, const char *fmt, ...)
{
    unsigned char *p;
    va_list args;
    va_start(args, fmt);
    p = xqc_vsprintf(buf, last, fmt, args);
    va_end(args);
    return p;
}


inline static xqc_bool_t
xqc_memeq(const void *s1, const void *s2, size_t n)
{
    return n == 0 || memcmp(s1, s2, n) == 0;
}

inline static xqc_bool_t
xqc_char_is_letter_or_number(char c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') 
        || (c >= '0' && c <= '9'))
    {
        return XQC_TRUE;
    }
    return XQC_FALSE;
}

static inline void
xqc_int_2_str(char *output, uint32_t val, xqc_int_t val_len)
{
    xqc_int_t i, tail;
    i = val_len - 1;
    while (val > 0 && i >= 0) {
        tail = val % 10;
        output[i] = '0' + tail;
        i--;
        val /= 10;
    }
    for (i; i >= 0; i--) {
        output[i] = '0';
    }
}

static inline uint32_t
xqc_str_2_int(char *str, xqc_int_t str_len)
{
    xqc_int_t i, tmp, result;
    result = 0;

    for (i = 0; i < str_len; i++) {
        result *= 10;
        tmp = str[i] - '0';
        result += tmp;
    }

    return result;
}

#endif /*_XQC_STR_H_INCLUDED_*/
