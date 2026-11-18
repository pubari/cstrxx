add_library(benchmark_options INTERFACE)
target_link_libraries(benchmark_options INTERFACE cstrxx)
target_link_options(benchmark_options INTERFACE -march=native)
target_compile_features(benchmark_options INTERFACE cxx_std_20) # need for <format>

add_executable(intset intset.cc benchmarking.h)
target_link_libraries(intset PRIVATE benchmark_options)
install(TARGETS intset RUNTIME DESTINATION bin)

add_executable(s_intset s_intset.cc benchmarking.h)
target_link_libraries(s_intset PRIVATE benchmark_options)
install(TARGETS s_intset RUNTIME DESTINATION bin)

add_executable(doubleset doubleset.cc benchmarking.h)
target_link_libraries(doubleset PRIVATE benchmark_options)
install(TARGETS doubleset RUNTIME DESTINATION bin)

add_executable(s_doubleset s_doubleset.cc benchmarking.h)
target_link_libraries(s_doubleset PRIVATE benchmark_options)
install(TARGETS s_doubleset RUNTIME DESTINATION bin)
