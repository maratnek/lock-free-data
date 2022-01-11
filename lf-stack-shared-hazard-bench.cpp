#include <benchmark/benchmark.h>

#include <iostream>
#include "threads-utils.hpp"
#include "lf-stack-shared-hazard.hpp"

static void BM_LFStack(benchmark::State &state)
{
    state.SetItemsProcessed(1000);
    for (auto _ : state)
    {
        lockFreeStack<size_t> lfStack;
        std::atomic<size_t> res = 0;

        auto l_func_load = [&lfStack, &res](size_t i = 0, size_t max = 0)
        {
            size_t result = 0;
            for (; i < max; i++)
            {
                lfStack.push(1);
                if (i % 2 == 0)
                {
                    auto temp_res = lfStack.pop();
                    if (temp_res)
                        result += *temp_res;
                }
            }
            res += result;
        };

        size_t count = 16'000'000;
        l_thread_data<decltype(l_func_load)>(l_func_load, count);
        std::cout << "Result " << res << std::endl;
    }
}
BENCHMARK(BM_LFStack)->Arg(1)->Unit(benchmark::kMillisecond);

#include "thread-safe-stack.hpp"
static void BM_ThreadSafeStack(benchmark::State &state)
{
    for (auto _ : state)
    {
        threadsafe_stack<size_t> lfStack;
        std::atomic<size_t> res = 0;

        auto l_func_load = [&lfStack, &res](size_t i = 0, size_t max = 0)
        {
            size_t result = 0;
            size_t temp_res = 0;
            for (; i < max; i++)
            {
                lfStack.push(1);
                if (i % 2 == 0)
                {
                    lfStack.pop(temp_res);
                    result += temp_res;
                }
            }
            res += result;
        };

        // size_t count = 12'000'000;
        size_t count = 16'000'000;
        l_thread_data<decltype(l_func_load)>(l_func_load, count);
        std::cout << "Result " << res << std::endl;
    }
}
BENCHMARK(BM_ThreadSafeStack)->Arg(1)->Unit(benchmark::kMillisecond);
// Run the benchmark
BENCHMARK_MAIN();