#include <benchmark/benchmark.h>

#include <iostream>
#include "threads-utils.hpp"

#include <boost/lockfree/stack.hpp>

static void BM_LFStack(benchmark::State& state) {
    for (auto _ : state) 
    {
        size_t count = 12'000'000;
        boost::lockfree::stack<std::string> lfStack(count);
        std::atomic<size_t> res = 0;

        auto l_func_load = [&lfStack, &res](size_t i = 0, size_t max = 0)
        {
            size_t result = 0;
            std::string temp_res;
            for (; i < max; i++)
            {
                lfStack.push(std::to_string(i) + "Hello world");
                if (i % 2 == 0)
                {
                    lfStack.pop(temp_res);
                    ++result;
                }
            }
            res += result;
        };

        l_thread_data<decltype(l_func_load)>(l_func_load, count);
        std::cout << "Result " << res << std::endl;
    }
}
BENCHMARK(BM_LFStack)->Arg(1)->Unit(benchmark::kMillisecond);

#include "thread-safe-stack.hpp"
static void BM_ThreadSafeStack(benchmark::State& state) {
    for (auto _ : state) 
    {
        threadsafe_stack<std::string> lfStack;
        std::atomic<size_t> res = 0;

        auto l_func_load = [&lfStack, &res](size_t i = 0, size_t max = 0)
        {
            size_t result = 0;
            std::string temp_res;
            for (; i < max; i++)
            {
                lfStack.push(std::to_string(i) + "Hello world");
                if (i % 2 == 0)
                {
                    lfStack.pop(temp_res);
                    ++result;
                }
            }
            res += result;
        };

        size_t count = 12'000'000;
        l_thread_data<decltype(l_func_load)>(l_func_load, count);
        std::cout << "Result " << res << std::endl;
    }
}
BENCHMARK(BM_ThreadSafeStack)->Arg(1)->Unit(benchmark::kMillisecond);
// Run the benchmark
BENCHMARK_MAIN();