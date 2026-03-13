#ifndef CSTRXX_H
#define CSTRXX_H

#include <ctime>
#include <cstring>
#include <stdexcept>
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
    return cstrxx_strset(buf, pos, str, strlen(str));
}

inline size_t cstrxx_strrset(char* buf, size_t pos, const char* str, size_t str_size, size_t nsize, char c)
{
    if (str_size < 1)
    {
        cstrxx_charspan(buf, pos, c, nsize);
        return nsize;
    }
    cstrxx_charspan(buf, pos, c, nsize - str_size);
    cstrxx_strset(buf, pos + nsize - str_size, str, str_size);
    return nsize;
}

inline size_t cstrxx_strrset(char* buf, size_t pos, const char* str, size_t nsize, char c)
{
    return cstrxx_strrset(buf, pos, str, strlen(str), nsize, c);
}

inline size_t cstrxx_strrset(char* buf, size_t pos, const char* str, size_t str_size, size_t nsize)
{
    return cstrxx_strrset(buf, pos, str, str_size, nsize, ' ');
}

inline size_t cstrxx_strrset(char* buf, size_t pos, const char* str, size_t nsize)
{
    return cstrxx_strrset(buf, pos, str, strlen(str), nsize, ' ');
}

inline size_t cstrxx_strlset(char* buf, size_t pos, const char* str, size_t str_size, size_t nsize, char c)
{
    if (str_size < 1)
    {
        cstrxx_charspan(buf, pos, c, nsize);
        return nsize;
    }
    cstrxx_strset(buf, pos, str, str_size);
    cstrxx_charspan(buf, pos + str_size, c, nsize - str_size);
    return nsize;
}

inline size_t cstrxx_strlset(char* buf, size_t pos, const char* str, size_t nsize, char c)
{
    return cstrxx_strlset(buf, pos, str, strlen(str), nsize, c);
}

inline size_t cstrxx_strlset(char* buf, size_t pos, const char* str, size_t str_size, size_t nsize)
{
    return cstrxx_strlset(buf, pos, str, str_size, nsize, ' ');
}

inline size_t cstrxx_strlset(char* buf, size_t pos, const char* str, size_t nsize)
{
    return cstrxx_strlset(buf, pos, str, strlen(str), nsize, ' ');
}

inline size_t cstrxx_strcset(char* buf, size_t pos, const char* str, size_t str_size, size_t nsize, char c)
{
    if (str_size < 1)
    {
        return cstrxx_charspan(buf, pos, c, nsize);
    }
    size_t sidepad = (nsize - str_size) / 2;
    cstrxx_charspan(buf, pos, c, sidepad);
    cstrxx_strset(buf, pos + sidepad, str, str_size);
    cstrxx_charspan(buf, pos + sidepad + str_size, c, sidepad + (nsize - str_size) % 2);
    return nsize;
}

inline size_t cstrxx_strcset(char* buf, size_t pos, const char* str, size_t str_size, size_t nsize)
{
    return cstrxx_strcset(buf, pos, str, str_size, nsize, ' ');
}

// NUMBER SIGN INSERTIONS

static inline size_t _cstrxx_floatsign(char* buf, size_t pos, double num)
{
    return cstrxx_charset(buf, pos, num >= 0.0 ? '+' : '-');
}

static inline size_t _cstrxx_intsign(char* buf, size_t pos, int64_t num)
{
    return cstrxx_charset(buf, pos, num >= 0 ? '+' : '-');
}

// INT INSERTIONS

static inline ptrdiff_t _cstrxx_int_to_char(char* buf, size_t buf_size, int64_t num)
{
    std::to_chars_result tcr = std::to_chars(buf, buf + buf_size, num);
    return tcr.ptr - buf;
}

inline size_t cstrxx_intset(char* buf, size_t pos, int64_t num)
{
    char fbuf[FBUF_SIZE];
    ptrdiff_t fbuf_size = _cstrxx_int_to_char(fbuf, FBUF_SIZE, num);
    return cstrxx_strset(buf, pos, fbuf, fbuf_size);
}

inline size_t cstrxx_intrset(char* buf, size_t pos, int64_t num, size_t nsize, char c)
{
    char fbuf[FBUF_SIZE];
    ptrdiff_t fbuf_size = _cstrxx_int_to_char(fbuf, FBUF_SIZE, num);
    return cstrxx_strrset(buf, pos, fbuf, fbuf_size, nsize, c);
}

inline size_t cstrxx_intrset(char* buf, size_t pos, int64_t num, size_t nsize)
{
    return cstrxx_intrset(buf, pos, num, nsize, ' ');
}

inline size_t cstrxx_intlset(char* buf, size_t pos, int64_t num, size_t nsize, char c)
{
    char fbuf[FBUF_SIZE];
    ptrdiff_t fbuf_size = _cstrxx_int_to_char(fbuf, FBUF_SIZE, num);
    return cstrxx_strlset(buf, pos, fbuf, fbuf_size, nsize, c);
}

inline size_t cstrxx_intlset(char* buf, size_t pos, int64_t num, size_t nsize)
{
    return cstrxx_intlset(buf, pos, num, nsize, ' ');
}

inline size_t cstrxx_intcset(char* buf, size_t pos, int64_t num, size_t nsize, char c)
{
    char fbuf[FBUF_SIZE];
    ptrdiff_t fbuf_size = _cstrxx_int_to_char(fbuf, FBUF_SIZE, num);
    return cstrxx_strcset(buf, pos, fbuf, fbuf_size, nsize, c);
}

inline size_t cstrxx_intcset(char* buf, size_t pos, int64_t num, size_t nsize)
{
    return cstrxx_intcset(buf, pos, num, nsize, ' ');
}

// FLOAT INSERTIONS

static inline ptrdiff_t _cstrxx_floatf(char* buf, size_t buf_size, double num, int64_t ffixed)
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

inline size_t cstrxx_floatfset(char* buf, size_t pos, double num, int64_t ffixed)
{
    char fbuf[FBUF_SIZE];
    ptrdiff_t fbuf_size = _cstrxx_floatf(fbuf, FBUF_SIZE, num, ffixed);
    return cstrxx_strset(buf, pos, fbuf, fbuf_size);
}

inline size_t cstrxx_floatfset(char* buf, size_t pos, double num)
{
    return cstrxx_floatfset(buf, pos, num, -1);
}

inline size_t cstrxx_floatfrset(char* buf, size_t pos, double num, int64_t ffixed, size_t nsize, char c)
{
    char fbuf[FBUF_SIZE];
    ptrdiff_t fbuf_size = _cstrxx_floatf(fbuf, FBUF_SIZE, num, ffixed);
    size_t pad_size = nsize - fbuf_size;
    return cstrxx_strrset(buf, pos, fbuf, fbuf_size, nsize, c);
}

inline size_t cstrxx_floatfrset(char* buf, size_t pos, double num, size_t nsize, char c)
{
    return cstrxx_floatfrset(buf, pos, num, -1, nsize, c);
}

inline size_t cstrxx_floatfrset(char* buf, size_t pos, double num, int64_t ffixed, size_t nsize)
{
    return cstrxx_floatfrset(buf, pos, num, ffixed, nsize, ' ');
}

inline size_t cstrxx_floatfrset(char* buf, size_t pos, double num, size_t nsize)
{
    return cstrxx_floatfrset(buf, pos, num, -1, nsize, ' ');
}

inline size_t cstrxx_floatflset(char* buf, size_t pos, double num, int64_t ffixed, size_t nsize, char c)
{
    char fbuf[FBUF_SIZE];
    ptrdiff_t fbuf_size = _cstrxx_floatf(fbuf, FBUF_SIZE, num, ffixed);
    size_t pad_size = nsize - fbuf_size;
    return cstrxx_strlset(buf, pos, fbuf, fbuf_size, nsize, c);
}

inline size_t cstrxx_floatflset(char* buf, size_t pos, double num, size_t nsize, char c)
{
    return cstrxx_floatflset(buf, pos, num, -1, nsize, c);
}

inline size_t cstrxx_floatflset(char* buf, size_t pos, double num, int64_t ffixed, size_t nsize)
{
    return cstrxx_floatflset(buf, pos, num, ffixed, nsize, ' ');
}

inline size_t cstrxx_floatflset(char* buf, size_t pos, double num, size_t nsize)
{
    return cstrxx_floatflset(buf, pos, num, -1, nsize, ' ');
}

inline size_t cstrxx_floatfcset(char* buf, size_t pos, double num, int64_t ffixed, size_t nsize, char c)
{
    char fbuf[FBUF_SIZE];
    ptrdiff_t fbuf_size = _cstrxx_floatf(fbuf, FBUF_SIZE, num, ffixed);
    return cstrxx_strcset(buf, pos, fbuf, fbuf_size, nsize, c);
}

inline size_t cstrxx_floatfcset(char* buf, size_t pos, double num, int64_t ffixed, size_t nsize)
{
    return cstrxx_floatfcset(buf, pos, num, ffixed, nsize, ' ');
}

inline size_t cstrxx_floatfcset(char* buf, size_t pos, double num, size_t nsize, char c)
{
    return cstrxx_floatfcset(buf, pos, num, -1, nsize, c);
}

inline size_t cstrxx_floatfcset(char* buf, size_t pos, double num, size_t nsize)
{
    return cstrxx_floatfcset(buf, pos, num, -1, nsize, ' ');
}

inline size_t cstrxx_sfloatfset(char* buf, size_t pos, double num, int64_t ffixed)
{
    char fbuf[FBUF_SIZE];
    ptrdiff_t fbuf_size = _cstrxx_floatf(fbuf, FBUF_SIZE, (num < 0 ? num * -1 : num), ffixed);
    return _cstrxx_floatsign(buf, pos, num) + cstrxx_strset(buf, pos + 1, fbuf, fbuf_size);
}

inline size_t cstrxx_sfloatfset(char* buf, size_t pos, double num)
{
    return cstrxx_sfloatfset(buf, pos, num, -1);
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

inline size_t cstrxx_dsset(char* buf, size_t pos, time_t second, int tz_offset)
{
    time_t tz_second = second + tz_offset * 3600;
    if (tz_second > 86399)
    {
        tz_second -= 86400;
    }
    else if (tz_second < 0)
    {
        tz_second += 86400;
    }

    time_t clock_hour = tz_second / 3600;
    time_t clock_minute = tz_second % 3600 / 60;
    time_t clock_second = tz_second % 3600 % 60;

    size_t dts_size = 0;
    dts_size += cstrxx_intrset(buf, pos, clock_hour, 2, '0');
    dts_size += cstrxx_charset(buf, pos + dts_size, ':');
    dts_size += cstrxx_intrset(buf, pos + dts_size, clock_minute, 2, '0');
    dts_size += cstrxx_charset(buf, pos + dts_size, ':');
    dts_size += cstrxx_intrset(buf, pos + dts_size, clock_second, 2, '0');
    return dts_size;
}

inline size_t cstrxx_dsset(char* buf, size_t pos, time_t second)
{
    return cstrxx_dsset(buf, pos, second, 0);
}

// FROM STRING CONVERTIONS

inline double cstrxx_floatget(const char* str, size_t str_size)
{
    double fvalue;
    std::from_chars_result fcr = std::from_chars(str, str + str_size, fvalue);
    if (fcr.ec == std::errc{})
    {
        return fvalue;
    }
    throw std::invalid_argument("could not convert str to float");
}

inline double cstrxx_floatget(const char* str)
{
    return cstrxx_floatget(str, strlen(str));
}

inline int64_t cstrxx_intget(const char* str, size_t str_size)
{
    int64_t ivalue;
    std::from_chars_result fcr = std::from_chars(str, str + str_size, ivalue);
    if (fcr.ec == std::errc{})
    {
        return ivalue;
    }
    throw std::invalid_argument("could not convert str to int");
}

inline int64_t cstrxx_intget(const char* str)
{
    return cstrxx_intget(str, strlen(str));
}

#endif // CSTRXX_H
