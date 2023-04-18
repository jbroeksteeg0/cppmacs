#pragma once

namespace Tester {
namespace Tests{ 
void test_rope();
void test_rope_simple();
void test_rope_randomised();
}
namespace Benchmarks {
void benchmark_rope(int seed, int iters);
void benchmark_vec(int seed, int iters);
}
void run_tests();
}    // namespace Tester
