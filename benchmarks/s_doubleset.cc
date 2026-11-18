#include "cstrxx.h"
#include "benchmarking.h"

#include <cstdio>
#include <format>

inline constexpr double N1 = +0.74;
inline constexpr double N2 = -0.26;
inline constexpr double N3 = +0.523;
inline constexpr double N4 = -0.474;
inline constexpr double N5 = +0.34;

void cstdio_benchmark(char* buf, size_t buf_size)
{
    /// NOTE: printf has no centered variation
    snprintf(buf, buf_size, "%+.3f|%+.3f|%+8.3f|%-+8.3f|%+8.3f", N1, N2, N3, N4, N5);
}

void format_benchmark(char* buf, size_t buf_size)
{
   std::format_to_n(buf, buf_size, "{:+.3f}|{:+.3f}|{:+8.3f}|{:<+8.3f}|{:^+8.3f}", N1, N2, N3, N4, N5);
}

void cstrxx_benchmark(char* buf, size_t buf_size)
{
    size_t offset = 0;
    offset += cstrxx_s_doubleset(buf, offset, N1, 3);
    offset += cstrxx_charset_1(buf, offset, '|');
    offset += cstrxx_s_doubleset(buf, offset, N2, 3);
    offset += cstrxx_charset_1(buf, offset, '|');
    offset += cstrxx_s_doubleset_r(buf, offset, N3, 3, 8, ' ');
    offset += cstrxx_charset_1(buf, offset, '|');
    offset += cstrxx_s_doubleset_l(buf, offset, N4, 3, 8, ' ');
    offset += cstrxx_charset_1(buf, offset, '|');
    offset += cstrxx_s_doubleset_c(buf, offset, N5, 3, 8, ' ');
    offset += cstrxx_charset_1(buf, offset, '\0');
}

int main(void)
{
    BENCHMARK(cstdio_benchmark);
    BENCHMARK(format_benchmark);
    BENCHMARK(cstrxx_benchmark);
}
