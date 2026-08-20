add_library(benchmark_options INTERFACE)
target_link_libraries(benchmark_options INTERFACE cstrxx)
target_link_options(benchmark_options INTERFACE -march=native)
target_compile_features(benchmark_options INTERFACE cxx_std_20)


add_executable(integers integers.cc benchmarking.h)
target_link_libraries(integers PRIVATE benchmark_options)
install(TARGETS integers RUNTIME DESTINATION bin)
