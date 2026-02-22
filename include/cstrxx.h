#ifndef CSTRXX_H
#define CSTRXX_H

#include <ctime>
#include <cstring>
#include <charconv>

constexpr const size_t FBUF_SIZE = 64;

// CHAR INSERTIONS

inline size_t cstrxx_charset(char* buf, size_t pos, char c)
{
    buf[pos] = c;
    return 1;
}

inline size_t cstrxx_dcharset(char* buf, size_t pos, char c1, char c2)
{
    return cstrxx_charset(buf, pos, c1) + cstrxx_charset(buf, pos + 1, c2);
}

inline size_t cstrxx_tcharset(char* buf, size_t pos, char c1, char c2, char c3)
{
    return cstrxx_charset(buf, pos, c1) + cstrxx_charset(buf, pos + 1, c2) + cstrxx_charset(buf, pos + 2, c3);
}

inline size_t cstrxx_charspan(char* buf, size_t pos, char c, size_t count)
{
    memset(buf + pos, c, count);
    return count;
}

// STRING INSERTIONS

inline size_t cstrxx_strset(char* buf, size_t pos, const char* str, size_t str_size)
{
    if (str_size < 1)
    {
        return 0;
    }
    memcpy(buf + pos, str, str_size);
    return str_size;
}

inline size_t cstrxx_strset(char* buf, size_t pos, const char* str)
{
    size_t str_size = strlen(str);
    return cstrxx_strset(buf, pos, str, str_size);
}

inline size_t cstrxx_strrset(char* buf, size_t pos, const char* str, size_t nsize, char c)
{
    size_t str_size = strlen(str);
    if (str_size < 1)
    {
        cstrxx_charspan(buf, pos, c, nsize);
        return nsize;
    }
    cstrxx_charspan(buf, pos, c, nsize - str_size);
    cstrxx_strset(buf, pos + nsize - str_size, str, str_size);
    return nsize;
}

inline size_t cstrxx_strrset(char* buf, size_t pos, const char* str, size_t nsize)
{
    return cstrxx_strrset(buf, pos, str, nsize, ' ');
}

inline size_t cstrxx_strlset(char* buf, size_t pos, const char* str, size_t nsize, char c)
{
    size_t str_size = strlen(str);
    if (str_size < 1)
    {
        cstrxx_charspan(buf, pos, c, nsize);
        return nsize;
    }
    cstrxx_strset(buf, pos, str, str_size);
    cstrxx_charspan(buf, pos + str_size, c, nsize - str_size);
    return nsize;
}

inline size_t cstrxx_strlset(char* buf, size_t pos, const char* str, size_t nsize)
{
    return cstrxx_strlset(buf, pos, str, nsize, ' ');
}

// NUMBER SIGN INSERTIONS

static inline size_t _cstrxx_floatsign(char* buf, size_t pos, double num)
{
    return cstrxx_charset(buf, pos, num >= 0.0 ? '+' : '-');
}

static inline size_t _cstrxx_intsign(char* buf, size_t pos, int num)
{
    return cstrxx_charset(buf, pos, num >= 0 ? '+' : '-');
}

// INT INSERTIONS

static inline size_t _cstrxx_int_to_char(char* buf, size_t buf_size, int num)
{
    std::to_chars_result tcr = std::to_chars(buf, buf + buf_size, num);
    return tcr.ptr - buf;
}

inline size_t cstrxx_intset(char* buf, size_t pos, int num)
{
    char fbuf[FBUF_SIZE];
    size_t fbuf_size = _cstrxx_int_to_char(fbuf, FBUF_SIZE, num);
    return cstrxx_strset(buf, pos, fbuf, fbuf_size);
}

inline size_t cstrxx_intrset(char* buf, size_t pos, int num, size_t nsize, char c)
{
    char fbuf[FBUF_SIZE];
    size_t fbuf_size = _cstrxx_int_to_char(fbuf, FBUF_SIZE, num);
    return cstrxx_charspan(buf, pos, c, nsize - fbuf_size) + cstrxx_strset(buf, pos + nsize - fbuf_size, fbuf, fbuf_size);
}

inline size_t cstrxx_intrset(char* buf, size_t pos, int num, size_t nsize)
{
    return cstrxx_intrset(buf, pos, num, nsize, ' ');
}

inline size_t cstrxx_intlset(char* buf, size_t pos, int num, size_t nsize, char c)
{
    char fbuf[FBUF_SIZE];
    size_t fbuf_size = _cstrxx_int_to_char(fbuf, FBUF_SIZE, num);
    return cstrxx_strset(buf, pos, fbuf, fbuf_size) + cstrxx_charspan(buf, pos + fbuf_size, c, nsize - fbuf_size);
}

inline size_t cstrxx_intlset(char* buf, size_t pos, int num, size_t nsize)
{
    return cstrxx_intlset(buf, pos, num, nsize, ' ');
}

// FLOAT INSERTIONS

static inline size_t _cstrxx_floatf(char* buf, size_t buf_size, double num, int ffixed)
{
    std::to_chars_result tcr;
    if (ffixed < 0)
    {
        tcr = std::to_chars(buf, (buf) + buf_size, num, std::chars_format::general, 14);
    }
    else
    {
        tcr = std::to_chars(buf, (buf) + buf_size, num, std::chars_format::fixed, ffixed);
    }
    return tcr.ptr - buf;
}

inline size_t cstrxx_floatfset(char* buf, size_t pos, double num, int ffixed)
{
    char fbuf[FBUF_SIZE];
    size_t fbuf_size = _cstrxx_floatf(fbuf, FBUF_SIZE, num, ffixed);
    return cstrxx_strset(buf, pos, fbuf, fbuf_size);
}

inline size_t cstrxx_floatfset(char* buf, size_t pos, double num)
{
    return cstrxx_floatfset(buf, pos, num, -1);
}

inline size_t cstrxx_floatfrset(char* buf, size_t pos, double num, int ffixed, char c, int nsize)
{
    char fbuf[FBUF_SIZE];
    size_t fbuf_size = _cstrxx_floatf(fbuf, FBUF_SIZE, num, ffixed);
    size_t pad_size = nsize - fbuf_size;
    return cstrxx_charspan(buf, pos, c, pad_size) + cstrxx_strset(buf, pos + pad_size, fbuf, fbuf_size);
}

inline size_t cstrxx_floatfrset(char* buf, size_t pos, double num, char c, int nsize)
{
    return cstrxx_floatfrset(buf, pos, num, -1, c, nsize);
}

inline size_t cstrxx_floatfrset(char* buf, size_t pos, double num, int ffixed, int nsize)
{
    return cstrxx_floatfrset(buf, pos, num, ffixed, ' ', nsize);
}

inline size_t cstrxx_floatfrset(char* buf, size_t pos, double num, int nsize)
{
    return cstrxx_floatfrset(buf, pos, num, -1, ' ', nsize);
}

inline size_t cstrxx_floatflset(char* buf, size_t pos, double num, int ffixed, char c, int nsize)
{
    char fbuf[FBUF_SIZE];
    size_t fbuf_size = _cstrxx_floatf(fbuf, FBUF_SIZE, num, ffixed);
    size_t pad_size = nsize - fbuf_size;
    return cstrxx_strset(buf, pos, fbuf, fbuf_size) + cstrxx_charspan(buf, pos + fbuf_size, c, pad_size);
}

inline size_t cstrxx_floatflset(char* buf, size_t pos, double num, char c, int nsize)
{
    return cstrxx_floatflset(buf, pos, num, -1, c, nsize);
}

inline size_t cstrxx_floatflset(char* buf, size_t pos, double num, int ffixed, int nsize)
{
    return cstrxx_floatflset(buf, pos, num, ffixed, ' ', nsize);
}

inline size_t cstrxx_floatflset(char* buf, size_t pos, double num, int nsize)
{
    return cstrxx_floatflset(buf, pos, num, -1, ' ', nsize);
}

inline size_t cstrxx_sfloatfset(char* buf, size_t pos, double num, int ffixed = -1)
{
    char fbuf[FBUF_SIZE];
    size_t fbuf_size = _cstrxx_floatf(fbuf, FBUF_SIZE, (num < 0 ? num * -1 : num), ffixed);
    pos += _cstrxx_floatsign(buf, pos, num);
    return cstrxx_strset(buf, pos, fbuf, fbuf_size) + 1;
}

// DATETIME INSERTIONS

inline size_t cstrxx_dtset(char* buf, size_t pos, time_t epoch)
{
    struct tm ts;
    localtime_r(&epoch, &ts);
    size_t dt_size = 0;
    dt_size += cstrxx_intrset(buf, pos, 1900 + ts.tm_year, 4, '0');
    dt_size += cstrxx_charset(buf, pos + dt_size, '-');
    dt_size += cstrxx_intrset(buf, pos + dt_size, ts.tm_mon + 1, 2, '0');
    dt_size += cstrxx_charset(buf, pos + dt_size, '-');
    dt_size += cstrxx_intrset(buf, pos + dt_size, ts.tm_mday, 2, '0');
    dt_size += cstrxx_charset(buf, pos + dt_size, ' ');
    dt_size += cstrxx_intrset(buf, pos + dt_size, ts.tm_hour, 2, '0');
    dt_size += cstrxx_charset(buf, pos + dt_size, ':');
    dt_size += cstrxx_intrset(buf, pos + dt_size, ts.tm_min, 2, '0');
    dt_size += cstrxx_charset(buf, pos + dt_size, ':');
    dt_size += cstrxx_intrset(buf, pos + dt_size, ts.tm_sec, 2, '0');
    return dt_size;
}

#endif // CSTRXX_H
