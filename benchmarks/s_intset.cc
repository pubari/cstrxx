#include "cstrxx.h"
#include "benchmarking.h"

#include <cstdio>
#include <format>

inline constexpr int N1 = 1;
inline constexpr int N2 = -30;
inline constexpr int N3 = -100;
inline constexpr int N4 = 1000;
inline constexpr int N5 = 0;

void cstdio_benchmark(char* buf, size_t buf_size)
{
    /// NOTE: printf has no centered variation
    snprintf(buf, buf_size, "%+d|%+d|%+8d|%-+8d|%+8d", N1, N2, N3, N4, N5);
}

void format_benchmark(char* buf, size_t buf_size)
{
   std::format_to_n(buf, buf_size, "{:+d}|{:+d}|{:+8d}|{:<+8d}|{:^+8d}", N1, N2, N3, N4, N5);
}

void cstrxx_benchmark(char* buf, size_t buf_size)
{
    size_t offset = 0;
    offset += cstrxx_s_intset(buf, offset, N1);
    offset += cstrxx_charset_1(buf, offset, '|');
    offset += cstrxx_s_intset(buf, offset, N2);
    offset += cstrxx_charset_1(buf, offset, '|');
    offset += cstrxx_s_intset_r(buf, offset, N3, 8);
    offset += cstrxx_charset_1(buf, offset, '|');
    offset += cstrxx_s_intset_l(buf, offset, N4, 8);
    offset += cstrxx_charset_1(buf, offset, '|');
    offset += cstrxx_s_intset_c(buf, offset, N5, 8);
    offset += cstrxx_charset_1(buf, offset, '\0');
}

int main(void)
{
    BENCHMARK(cstdio_benchmark);
    BENCHMARK(format_benchmark);
    BENCHMARK(cstrxx_benchmark);
}
