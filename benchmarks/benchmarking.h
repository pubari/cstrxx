#ifndef CSTRXX_BENCHMARKING_H
#define CSTRXX_BENCHMARKING_H

#include <cstdint>
#include <chrono>

constexpr size_t BENCHMARK_STR_SIZE = 512;
constexpr size_t WARM_UP_ITER       = 10000;
constexpr size_t BENCHMARK_ITER     = 10000000;

template <typename F>
void run_benchmark(F benchmark, const char* f_name)
{
    char benchmark_str[BENCHMARK_STR_SIZE];
    for (size_t i = 0; i < WARM_UP_ITER; ++i)
    {
        benchmark(benchmark_str, BENCHMARK_STR_SIZE);
    }

    const auto time_start = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();

    for (size_t i = 0; i < BENCHMARK_ITER; ++i)
    {
        benchmark(benchmark_str, BENCHMARK_STR_SIZE);
    }

    const auto time_end = std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();

    const auto elapsed_ns = time_end - time_start;
    printf("%s\nResult:\"%s\"\nElapsed time: %11lld ns\n\n", f_name, benchmark_str, elapsed_ns);
}

#define BENCHMARK(f) run_benchmark(f, #f)

#endif // CSTRXX_BENCHMARKING_H
