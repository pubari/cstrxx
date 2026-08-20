#include "cstrxx.h"
#include "benchmarking.h"

void cstdio_benchmark(char* buf, size_t buf_size)
{
    snprintf(buf, buf_size, "%d %d %d %d %d", 1, 2, 3, 4, 5);
}

void format_benchmark(char* buf, size_t buf_size)
{
   std::format_to_n(buf, buf_size, "{} {} {} {} {}", 1, 2, 3, 4, 5);
}

void cstrxx_benchmark(char* buf, size_t buf_size)
{
    size_t offset = 0;
    offset += cstrxx_intset(buf, offset, 1);
    offset += cstrxx_charset_1(buf, offset, ' ');
    offset += cstrxx_intset(buf, offset, 2);
    offset += cstrxx_charset_1(buf, offset, ' ');
    offset += cstrxx_intset(buf, offset, 3);
    offset += cstrxx_charset_1(buf, offset, ' ');
    offset += cstrxx_intset(buf, offset, 4);
    offset += cstrxx_charset_1(buf, offset, ' ');
    offset += cstrxx_intset(buf, offset, 5);
    offset += cstrxx_charset_1(buf, offset, '\0');
}

int main(void)
{
    BENCHMARK(cstdio_benchmark);
    BENCHMARK(format_benchmark);
    BENCHMARK(cstrxx_benchmark);
}
