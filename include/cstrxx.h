#ifndef CSTRXX_H
#define CSTRXX_H

#include <ctime>
#include <cstring>
#include <stdexcept>
#include <charconv>

inline constexpr size_t    LOCALBUF_SIZE       = 64;

inline constexpr char      DEFAULT_PADC        = ' ';
inline constexpr int64_t   DEFAULT_FFIXED      = -1;
inline constexpr int32_t   DEFAULT_TZ_OFFSET   = 0;


///////// CHARACTERS OPERATIONS \\\\\\\\\


/**
 * Insert 1 char into C-string buffer
 *
 * @param buf   C-string buffer
 * @param pos   Char position in `buf` buffer from where insert `c` character
 * @param c     Character to insert
 *
 * @return      Number of written characters
**/
inline size_t cstrxx_charset_1(char* buf, size_t pos, char c)
{
    buf[pos] = c;
    return 1;
}

/**
 * Insert consequtively 2 chars into C-string buffer
 *
 * @param buf   C-string buffer
 * @param pos   Char position in `buf` buffer from where insert `c1` and `c2` characters
 * @param c1    First character to insert
 * @param c22   Second character to insert
 *
 * @return      Number of written characters
**/
inline size_t cstrxx_charset_2(char* buf, size_t pos, char c1, char c2)
{
    return cstrxx_charset_1(buf, pos, c1) + cstrxx_charset_1(buf, pos + 1, c2);
}

/**
 * Insert consequtively 3 chars into C-string buffer
 *
 * @param buf   C-string buffer
 * @param pos   Char position in `buf` buffer from where insert `c1`, `c2, and `c3` characters
 * @param c1    First character to insert
 * @param c2    Second character to insert
 * @param c3    Third character to insert
 *
 * @return      Number of written characters
**/
inline size_t cstrxx_charset_3(char* buf, size_t pos, char c1, char c2, char c3)
{
    return cstrxx_charset_1(buf, pos, c1) + cstrxx_charset_1(buf, pos + 1, c2) + cstrxx_charset_1(buf, pos + 2, c3);
}

/**
 * Insert span of characters into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where insert the span
 * @param c         Span character
 * @param count     Number of `c` characters to span
 *
 * @return          Number of written characters
 *
 * @note        If span is less then 4 characters, it performs basic char insertion
**/
inline size_t cstrxx_charspan(char* buf, size_t pos, char c, size_t count)
{
    switch(count)
    {
        case 0: return 0;
        case 1: return cstrxx_charset_1(buf, pos, c);
        case 2: return cstrxx_charset_2(buf, pos, c, c);
        case 3: return cstrxx_charset_3(buf, pos, c, c, c);
    }
    memset(buf + pos, c, count);
    return count;
}


///////// BASIC STRING OPERATIONS \\\\\\\\\


/**
 * Insert other C-string into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 * @param str_size  Other C-string size
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_strset(char* buf, size_t pos, const char* str, size_t str_size)
{
    if (str_size < 1)
    {
        return 0;
    }
    memcpy(buf + pos, str, str_size);
    return str_size;
}

/**
 * Insert other C-string into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 *
 * @return          Number of written characters
 *
 * @note    Finds out `str` size on it's own.
 *              To mitigate strlen() overhead use overload with str_size parameter.
**/
inline size_t cstrxx_strset(char* buf, size_t pos, const char* str)
{
    return cstrxx_strset(buf, pos, str, strlen(str));
}


///////// PADDED STRING OPERATIONS \\\\\\\\\


/**
 * Insert other C-string aligned to the right with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 * @param str_size  Other C-string size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_strset_r(char* buf, size_t pos, const char* str, size_t str_size, size_t pad_size, char padc)
{
    if (str_size < 1)
    {
        return cstrxx_charspan(buf, pos, padc, pad_size);
    }
    const size_t pad_left_size = pad_size - str_size;
    cstrxx_charspan(buf, pos, padc, pad_left_size);
    cstrxx_strset(buf, pos + pad_left_size, str, str_size);
    return pad_size;
}

/**
 * Insert other C-string aligned to the right with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
 *
 * @note    Finds out `str` size on it's own.
 *              To mitigate strlen() overhead use overload with str_size parameter.
**/
inline size_t cstrxx_strset_r(char* buf, size_t pos, const char* str, size_t pad_size, char padc)
{
    return cstrxx_strset_r(buf, pos, str, strlen(str), pad_size, padc);
}

/**
 * Insert other C-string aligned to the right with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 * @param str_size  Other C-string size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_strset_r(char* buf, size_t pos, const char* str, size_t str_size, size_t pad_size)
{
    return cstrxx_strset_r(buf, pos, str, str_size, pad_size, DEFAULT_PADC);
}

/**
 * Insert other C-string aligned to the right with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Finds out `str` size on it's own.
 *              To mitigate strlen() overhead use overload with str_size parameter.
 *
 *          Uses default ' ' space as padded character.
**/
inline size_t cstrxx_strset_r(char* buf, size_t pos, const char* str, size_t pad_size)
{
    return cstrxx_strset_r(buf, pos, str, strlen(str), pad_size, DEFAULT_PADC);
}

/**
 * Insert other C-string aligned to the left with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 * @param str_size  Other C-string size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_strset_l(char* buf, size_t pos, const char* str, size_t str_size, size_t pad_size, char padc)
{
    if (str_size < 1)
    {
        return cstrxx_charspan(buf, pos, padc, pad_size);
    }
    const size_t pad_left_size = pad_size - str_size;
    cstrxx_strset(buf, pos, str, str_size);
    cstrxx_charspan(buf, pos + str_size, padc, pad_left_size);
    return pad_size;
}

/**
 * Insert other C-string aligned to the left with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
 *
 * @note    Finds out `str` size on it's own.
 *              To mitigate strlen() overhead use overload with str_size parameter.
**/
inline size_t cstrxx_strset_l(char* buf, size_t pos, const char* str, size_t pad_size, char padc)
{
    return cstrxx_strset_l(buf, pos, str, strlen(str), pad_size, padc);
}

/**
 * Insert other C-string aligned to the left with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 * @param str_size  Other C-string size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_strset_l(char* buf, size_t pos, const char* str, size_t str_size, size_t pad_size)
{
    return cstrxx_strset_l(buf, pos, str, str_size, pad_size, DEFAULT_PADC);
}

/**
 * Insert other C-string aligned to the left with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Finds out `str` size on it's own.
 *              To mitigate strlen() overhead use overload with str_size parameter.
 *
 *          Uses default ' ' space as padded character.
**/
inline size_t cstrxx_strset_l(char* buf, size_t pos, const char* str, size_t pad_size)
{
    return cstrxx_strset_l(buf, pos, str, strlen(str), pad_size, DEFAULT_PADC); 
}

/**
 * Insert other C-string aligned to the center with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 * @param str_size  Other C-string size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_strset_c(char* buf, size_t pos, const char* str, size_t str_size, size_t pad_size, char padc)
{
    if (str_size < 1)
    {
        return cstrxx_charspan(buf, pos, padc, pad_size);
    }
    const size_t pad_left_size = pad_size - str_size;
    const size_t pad_side_size = (pad_left_size) / 2;
    cstrxx_charspan(buf, pos, padc, pad_side_size);
    cstrxx_strset(buf, pos + pad_side_size, str, str_size);
    cstrxx_charspan(buf, pos + pad_side_size + str_size, padc, pad_side_size + pad_left_size % 2);
    return pad_size;
}

/**
 * Insert other C-string aligned to the center with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
 *
 * @note    Finds out `str` size on it's own.
 *              To mitigate strlen() overhead use overload with str_size parameter.
**/
inline size_t cstrxx_strset_c(char* buf, size_t pos, const char* str, size_t pad_size, char padc)
{
    return cstrxx_strset_c(buf, pos, str, strlen(str), pad_size, padc);
}

/**
 * Insert other C-string aligned to the center with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 * @param str_size  Other C-string size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_strset_c(char* buf, size_t pos, const char* str, size_t str_size, size_t pad_size)
{
    return cstrxx_strset_c(buf, pos, str, str_size, pad_size, DEFAULT_PADC);
}

/**
 * Insert other C-string aligned to the center with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert the other C-string
 * @param str       Other C-string
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Finds out `str` size on it's own.
 *              To mitigate strlen() overhead use overload with str_size parameter.
 *
 *          Uses default ' ' space as padded character.
**/
inline size_t cstrxx_strset_c(char* buf, size_t pos, const char* str, size_t pad_size)
{
    return cstrxx_strset_c(buf, pos, str, strlen(str), pad_size, DEFAULT_PADC);
}


///////// INTEGER NUMBERS OPERATIONS \\\\\\\\\


/// Private helper function
///
/// Converts integer into char representation using <charconv>
/// and finds it's size
///
///     buf         C-string buffer
///     buf_size    C-string buffer size
///     num         Integer number
///
///     Returns     Size of integer `num` in char representation
///
static inline ptrdiff_t _cstrxx_itos(char* buf, size_t buf_size, int64_t num)
{
    const std::to_chars_result tcr = std::to_chars(buf, buf + buf_size, num);
    return tcr.ptr - buf;
}

/// Private helper function
///
/// Inserts explicit sign to the `buf` depending on `num` value
///
///     buf         C-string buffer
///     buf_size    C-string buffer size
///     num         Integer number
///
///     Returns     Number of written characters
///
static inline size_t _cstrxx_intsign(char* buf, size_t pos, int64_t num)
{
    return cstrxx_charset_1(buf, pos, num >= 0 ? '+' : '-');
}

/**
 * Insert integer value into C-string buffer
 *
 * @param buf   C-string buffer
 * @param pos   Char position in `buf` buffer from where to insert integer number
 * @param num   Integer number
 *
 * @return      Number of written characters
**/
inline size_t cstrxx_intset(char* buf, size_t pos, int64_t num)
{
    char localbuf[LOCALBUF_SIZE];
    ptrdiff_t localbuf_size = _cstrxx_itos(localbuf, LOCALBUF_SIZE, num);
    return cstrxx_strset(buf, pos, localbuf, localbuf_size);
}

/**
 * Insert integer number into C-string buffer with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Integer number
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_s_intset(char* buf, size_t pos, int64_t num)
{
    char localbuf[LOCALBUF_SIZE];
    ptrdiff_t localbuf_size = _cstrxx_itos(localbuf, LOCALBUF_SIZE, (num < 0 ? num * -1 : num));
    return _cstrxx_intsign(buf, pos, num) + cstrxx_strset(buf, pos + 1, localbuf, localbuf_size);
}

/**
 * Insert integer number aligned to the right with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_s_intset_r(char* buf, size_t pos, int64_t num, size_t pad_size, char padc)
{
    char localbuf[LOCALBUF_SIZE];
    const size_t localbuf_size = cstrxx_s_intset(localbuf, 0, num);
    return cstrxx_strset_r(buf, pos, localbuf, localbuf_size, pad_size, padc);
}

/**
 * Insert integer number aligned to the right with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_s_intset_r(char* buf, size_t pos, int64_t num, size_t pad_size)
{
    return cstrxx_s_intset_r(buf, pos, num, pad_size, DEFAULT_PADC);
}

/**
 * Insert integer number aligned to the left with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_s_intset_l(char* buf, size_t pos, int64_t num, size_t pad_size, char padc)
{
    char localbuf[LOCALBUF_SIZE];
    const size_t localbuf_size = cstrxx_s_intset(localbuf, 0, num);
    return cstrxx_strset_l(buf, pos, localbuf, localbuf_size, pad_size, padc);
}

/**
 * Insert integer number aligned to the left with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_s_intset_l(char* buf, size_t pos, int64_t num, size_t pad_size)
{
    return cstrxx_s_intset_l(buf, pos, num, pad_size, DEFAULT_PADC);
}

/**
 * Insert integer number aligned to the center with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_s_intset_c(char* buf, size_t pos, int64_t num, size_t pad_size, char padc)
{
    char localbuf[LOCALBUF_SIZE];
    const size_t localbuf_size = cstrxx_s_intset(localbuf, 0, num);
    return cstrxx_strset_c(buf, pos, localbuf, localbuf_size, pad_size, padc);
}

/**
 * Insert integer number aligned to the center with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_s_intset_c(char* buf, size_t pos, int64_t num, size_t pad_size)
{
    return cstrxx_s_intset_c(buf, pos, num, pad_size, DEFAULT_PADC);
}

/**
 * Insert integer value aligned to the right with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert integer number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_intset_r(char* buf, size_t pos, int64_t num, size_t pad_size, char padc)
{
    char localbuf[LOCALBUF_SIZE];
    ptrdiff_t localbuf_size = _cstrxx_itos(localbuf, LOCALBUF_SIZE, num);
    return cstrxx_strset_r(buf, pos, localbuf, localbuf_size, pad_size, padc);
}

/**
 * Insert integer value aligned to the right with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert integer number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_intset_r(char* buf, size_t pos, int64_t num, size_t pad_size)
{
    return cstrxx_intset_r(buf, pos, num, pad_size, DEFAULT_PADC);
}

/**
 * Insert integer value aligned to the left with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert integer number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_intset_l(char* buf, size_t pos, int64_t num, size_t pad_size, char padc)
{
    char localbuf[LOCALBUF_SIZE];
    ptrdiff_t localbuf_size = _cstrxx_itos(localbuf, LOCALBUF_SIZE, num);
    return cstrxx_strset_l(buf, pos, localbuf, localbuf_size, pad_size, padc);
}

/**
 * Insert integer value aligned to the left with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert integer number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_intset_l(char* buf, size_t pos, int64_t num, size_t pad_size)
{
    return cstrxx_intset_l(buf, pos, num, pad_size, DEFAULT_PADC);
}

/**
 * Insert integer value aligned to the center with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert integer number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_intset_c(char* buf, size_t pos, int64_t num, size_t pad_size, char padc)
{
    char localbuf[LOCALBUF_SIZE];
    ptrdiff_t localbuf_size = _cstrxx_itos(localbuf, LOCALBUF_SIZE, num);
    return cstrxx_strset_c(buf, pos, localbuf, localbuf_size, pad_size, padc);
}

/**
 * Insert integer value aligned to the center with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert integer number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_intset_c(char* buf, size_t pos, int64_t num, size_t pad_size)
{
    return cstrxx_intset_c(buf, pos, num, pad_size, DEFAULT_PADC);
}


///////// FLOATING POINT NUMBERS OPERATIONS \\\\\\\\\


/// Private helper function
///
/// Converts double-precision float number into char representation using <charconv>
/// and finds it's size.
/// Additionally limits the fractional side
///
///     buf         C-string buffer
///     buf_size    C-string buffer size
///     num         Double-precision float number
///     ffixed      Fraction fixed size
///
///     Returns     Size of double-precious float number `num` in char representation
///
static inline ptrdiff_t _cstrxx_dtos(char* buf, size_t buf_size, double num, int64_t ffixed)
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

/// Private helper function
///
/// Inserts explicit sign to the `buf` depending on `num` value
///
///     buf         C-string buffer
///     buf_size    C-string buffer size
///     num         Double-precision float number
///
///     Returns     Number of written characters
///
static inline size_t _cstrxx_floatsign(char* buf, size_t pos, double num)
{
    return cstrxx_charset_1(buf, pos, num >= 0.0 ? '+' : '-');
}

/**
 * Insert double float number into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param ffixed    Fractional fixed size
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_doubleset(char* buf, size_t pos, double num, int64_t ffixed)
{
    char localbuf[LOCALBUF_SIZE];
    ptrdiff_t localbuf_size = _cstrxx_dtos(localbuf, LOCALBUF_SIZE, num, ffixed);
    return cstrxx_strset(buf, pos, localbuf, localbuf_size);
}

/**
 * Insert double float number into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 *
 * @return          Number of written characters
 *
 * @note    Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_doubleset(char* buf, size_t pos, double num)
{
    return cstrxx_doubleset(buf, pos, num, DEFAULT_FFIXED);
}

/**
 * Insert double float number into C-string buffer with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param ffixed    Fractional fixed size
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_s_doubleset(char* buf, size_t pos, double num, int64_t ffixed)
{
    char localbuf[LOCALBUF_SIZE];
    ptrdiff_t localbuf_size = _cstrxx_dtos(localbuf, LOCALBUF_SIZE, (num < 0 ? num * -1 : num), ffixed);
    return _cstrxx_floatsign(buf, pos, num) + cstrxx_strset(buf, pos + 1, localbuf, localbuf_size);
}

/**
 * Insert double float number into C-string buffer with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param ffixed    Fractional fixed size
 *
 * @return          Number of written characters
 *
 * @note    Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_s_doubleset(char* buf, size_t pos, double num)
{
    return cstrxx_s_doubleset(buf, pos, num, DEFAULT_FFIXED);
}

/**
 * Insert double float number aligned to the right with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param ffixed    Fractional fixed size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_s_doubleset_r(char* buf, size_t pos, double num, int64_t ffixed, size_t pad_size, char padc)
{
    char localbuf[LOCALBUF_SIZE];
    const size_t localbuf_size = cstrxx_s_doubleset(localbuf, 0, num, ffixed);
    return cstrxx_strset_r(buf, pos, localbuf, localbuf_size, pad_size, padc);
}

/**
 * Insert double float number aligned to the right with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
 *
 * @note    Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_s_doubleset_r(char* buf, size_t pos, double num, size_t pad_size, char padc)
{
    return cstrxx_s_doubleset_r(buf, pos, num, DEFAULT_FFIXED, pad_size, padc);
}

/**
 * Insert double float number aligned to the right with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param ffixed    Fractional fixed size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_s_doubleset_r(char* buf, size_t pos, double num, int64_t ffixed, size_t pad_size)
{
    return cstrxx_s_doubleset_r(buf, pos, num, ffixed, pad_size, DEFAULT_PADC);
}

/**
 * Insert double float number aligned to the right with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
 *          Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_s_doubleset_r(char* buf, size_t pos, double num, size_t pad_size)
{
    return cstrxx_s_doubleset_r(buf, pos, num, DEFAULT_FFIXED, pad_size, DEFAULT_PADC);
}

/**
 * Insert double float number aligned to the left with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param ffixed    Fractional fixed size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_s_doubleset_l(char* buf, size_t pos, double num, int64_t ffixed, size_t pad_size, char padc)
{
    char localbuf[LOCALBUF_SIZE];
    const size_t localbuf_size = cstrxx_s_doubleset(localbuf, 0, num, ffixed);
    return cstrxx_strset_l(buf, pos, localbuf, localbuf_size, pad_size, padc);
}

/**
 * Insert double float number aligned to the left with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
 *
 * @note    Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_s_doubleset_l(char* buf, size_t pos, double num, size_t pad_size, char padc)
{
    return cstrxx_s_doubleset_l(buf, pos, num, DEFAULT_FFIXED, pad_size, padc);
}

/**
 * Insert double float number aligned to the left with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param ffixed    Fractional fixed size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_s_doubleset_l(char* buf, size_t pos, double num, int64_t ffixed, size_t pad_size)
{
    return cstrxx_s_doubleset_l(buf, pos, num, ffixed, pad_size, DEFAULT_PADC);
}

/**
 * Insert double float number aligned to the left with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
 *          Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_s_doubleset_l(char* buf, size_t pos, double num, size_t pad_size)
{
    return cstrxx_s_doubleset_l(buf, pos, num, DEFAULT_FFIXED, pad_size, DEFAULT_PADC);
}

/**
 * Insert double float number aligned to the center with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param ffixed    Fractional fixed size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_s_doubleset_c(char* buf, size_t pos, double num, int64_t ffixed, size_t pad_size, char padc)
{
    char localbuf[LOCALBUF_SIZE];
    const size_t localbuf_size = cstrxx_s_doubleset(localbuf, 0, num, ffixed);
    return cstrxx_strset_c(buf, pos, localbuf, localbuf_size, pad_size, padc);
}
/**
 * Insert double float number aligned to the center with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
 *
 * @note    Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_s_doubleset_c(char* buf, size_t pos, double num, size_t pad_size, char padc)
{
    return cstrxx_s_doubleset_c(buf, pos, num, DEFAULT_FFIXED, pad_size, padc);
}

/**
 * Insert double float number aligned to the center with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param ffixed    Fractional fixed size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_s_doubleset_c(char* buf, size_t pos, double num, int64_t ffixed, size_t pad_size)
{
    return cstrxx_s_doubleset_c(buf, pos, num, ffixed, pad_size, DEFAULT_PADC);
}

/**
 * Insert double float number aligned to the center with padding into C-string buffer
 *   with explicit sign char
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
 *          Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_s_doubleset_c(char* buf, size_t pos, double num, size_t pad_size)
{
    return cstrxx_s_doubleset_c(buf, pos, num, DEFAULT_FFIXED, pad_size, DEFAULT_PADC);
}

/**
 * Insert double float number aligned to the right with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param ffixed    Fractional fixed size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_doubleset_r(char* buf, size_t pos, double num, int64_t ffixed, size_t pad_size, char padc)
{
    char localbuf[LOCALBUF_SIZE];
    ptrdiff_t localbuf_size = _cstrxx_dtos(localbuf, LOCALBUF_SIZE, num, ffixed);
    return cstrxx_strset_r(buf, pos, localbuf, localbuf_size, pad_size, padc);
}

/**
 * Insert double float number aligned to the right with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
 *
 * @note    Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_doubleset_r(char* buf, size_t pos, double num, size_t pad_size, char padc)
{
    return cstrxx_doubleset_r(buf, pos, num, DEFAULT_FFIXED, pad_size, padc);
}

/**
 * Insert double float number aligned to the right with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Integer number
 * @param ffixed    Fractional fixed size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_doubleset_r(char* buf, size_t pos, double num, int64_t ffixed, size_t pad_size)
{
    return cstrxx_doubleset_r(buf, pos, num, ffixed, pad_size, DEFAULT_PADC);
}

/**
 * Insert double float number aligned to the right with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
 *          Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_doubleset_r(char* buf, size_t pos, double num, size_t pad_size)
{
    return cstrxx_doubleset_r(buf, pos, num, DEFAULT_FFIXED, pad_size, DEFAULT_PADC);
}

/**
 * Insert double float number aligned to the left with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param ffixed    Fractional fixed size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_doubleset_l(char* buf, size_t pos, double num, int64_t ffixed, size_t pad_size, char padc)
{
    char localbuf[LOCALBUF_SIZE];
    ptrdiff_t localbuf_size = _cstrxx_dtos(localbuf, LOCALBUF_SIZE, num, ffixed);
    return cstrxx_strset_l(buf, pos, localbuf, localbuf_size, pad_size, padc);
}

/**
 * Insert double float number aligned to the left with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
 *
 * @note    Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_doubleset_l(char* buf, size_t pos, double num, size_t pad_size, char c)
{
    return cstrxx_doubleset_l(buf, pos, num, DEFAULT_FFIXED, pad_size, c);
}

/**
 * Insert double float number aligned to the left with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Integer number
 * @param ffixed    Fractional fixed size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_doubleset_l(char* buf, size_t pos, double num, int64_t ffixed, size_t pad_size)
{
    return cstrxx_doubleset_l(buf, pos, num, ffixed, pad_size, DEFAULT_PADC);
}

/**
 * Insert double float number aligned to the left with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
 *          Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_doubleset_l(char* buf, size_t pos, double num, size_t pad_size)
{
    return cstrxx_doubleset_l(buf, pos, num, DEFAULT_FFIXED, pad_size, DEFAULT_PADC);
}

/**
 * Insert double float number aligned to the center with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param ffixed    Fractional fixed size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_doubleset_c(char* buf, size_t pos, double num, int64_t ffixed, size_t pad_size, char padc)
{
    char localbuf[LOCALBUF_SIZE];
    ptrdiff_t localbuf_size = _cstrxx_dtos(localbuf, LOCALBUF_SIZE, num, ffixed);
    return cstrxx_strset_c(buf, pos, localbuf, localbuf_size, pad_size, padc);
}

/**
 * Insert double float number aligned to the center with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Double float number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 * @param padc      Padding character
 *
 * @return          Number of written characters
 *
 * @note    Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_doubleset_c(char* buf, size_t pos, double num, size_t pad_size, char padc)
{
    return cstrxx_doubleset_c(buf, pos, num, DEFAULT_FFIXED, pad_size, padc);
}

/**
 * Insert double float number aligned to the center with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Integer number
 * @param ffixed    Fractional fixed size
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
**/
inline size_t cstrxx_doubleset_c(char* buf, size_t pos, double num, int64_t ffixed, size_t pad_size)
{
    return cstrxx_doubleset_c(buf, pos, num, ffixed, pad_size, DEFAULT_PADC);
}

/**
 * Insert double float number aligned to the center with padding into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert double float number
 * @param num       Integer number
 * @param pad_size  Padded size. C-string of this size will be inserted into buffer
 *
 * @return          Number of written characters
 *
 * @note    Uses default ' ' space as padded character.
 *          Defaults ffixed to -1 (general formatting with length 14)
**/
inline size_t cstrxx_doubleset_c(char* buf, size_t pos, double num, size_t pad_size)
{
    return cstrxx_doubleset_c(buf, pos, num, DEFAULT_FFIXED, pad_size, DEFAULT_PADC);
}


///////// DATE AND TIME OPERATIONS \\\\\\\\\


/**
 * Insert datetime epoch in YYYY-MM-DD hh:mm::ss format into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert datetime epoch
 * @param epoch     Datetime epoch time
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_dtset(char* buf, size_t pos, time_t epoch)
{
    struct tm ts;
    localtime_r(&epoch, &ts);
    size_t dt_size = 0;
    dt_size += cstrxx_intset_r(buf, pos, 1900 + ts.tm_year, 4, '0');
    dt_size += cstrxx_charset_1(buf, pos + dt_size, '-');
    dt_size += cstrxx_intset_r(buf, pos + dt_size, ts.tm_mon + 1, 2, '0');
    dt_size += cstrxx_charset_1(buf, pos + dt_size, '-');
    dt_size += cstrxx_intset_r(buf, pos + dt_size, ts.tm_mday, 2, '0');
    dt_size += cstrxx_charset_1(buf, pos + dt_size, ' ');
    dt_size += cstrxx_intset_r(buf, pos + dt_size, ts.tm_hour, 2, '0');
    dt_size += cstrxx_charset_1(buf, pos + dt_size, ':');
    dt_size += cstrxx_intset_r(buf, pos + dt_size, ts.tm_min, 2, '0');
    dt_size += cstrxx_charset_1(buf, pos + dt_size, ':');
    dt_size += cstrxx_intset_r(buf, pos + dt_size, ts.tm_sec, 2, '0');
    return dt_size;
}

/**
 * Insert second of a day in hh:mm:ss format into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert second of a day
 * @param second    Second of a day
 * @param tz_offset Timezone offset
 *
 * @return          Number of written characters
**/
inline size_t cstrxx_sodset(char* buf, size_t pos, time_t second, int32_t tz_offset)
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
    dts_size += cstrxx_intset_r(buf, pos, clock_hour, 2, '0');
    dts_size += cstrxx_charset_1(buf, pos + dts_size, ':');
    dts_size += cstrxx_intset_r(buf, pos + dts_size, clock_minute, 2, '0');
    dts_size += cstrxx_charset_1(buf, pos + dts_size, ':');
    dts_size += cstrxx_intset_r(buf, pos + dts_size, clock_second, 2, '0');
    return dts_size;
}

/**
 * Insert second of a day in hh:mm:ss format into C-string buffer
 *
 * @param buf       C-string buffer
 * @param pos       Char position in `buf` buffer from where to insert second of a day
 * @param second    Second of a day
 *
 * @return          Number of written characters
 *
 * @note    Defaults tz_offset to UTC+0
**/
inline size_t cstrxx_sodset(char* buf, size_t pos, time_t second)
{
    return cstrxx_sodset(buf, pos, second, DEFAULT_TZ_OFFSET);
}


///////// FROM C-STRING OPERATIONS \\\\\\\\\


/**
 * Convert C-string to double float number
 *
 * @param str       C-string
 * @param str_size  C-string size
 *
 * @return          Double float number, otherwise for incorrect number throws error
**/
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

/**
 * Convert C-string to double float number
 *
 * @param str       C-string
 *
 * @return          Double float number, otherwise for incorrect number throws error
 *
 * @note    To mitigate strlen() overhead use overload with str_size parameter.
**/
inline double cstrxx_floatget(const char* str)
{
    return cstrxx_floatget(str, strlen(str));
}

/**
 * Convert C-string to 64 bit integer
 *
 * @param str       C-string
 * @param str_size  C-string size
 *
 * @return          64 bit integer, otherwise for incorrect number throws error
**/
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

/**
 * Convert C-string to 64 bit integer
 *
 * @param str       C-string
 *
 * @return          64 bit integer, otherwise for incorrect number throws error
 *
 * @note    To mitigate strlen() overhead use overload with str_size parameter.
**/
inline int64_t cstrxx_intget(const char* str)
{
    return cstrxx_intget(str, strlen(str));
}

#endif // CSTRXX_H
