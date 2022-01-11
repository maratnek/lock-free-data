#pragma once

#include <future>
#include <vector>

template <typename TLoadFunc>
auto l_thread_data = [](TLoadFunc func_load, size_t block_count = 1, size_t add_threads = 0)
{
    std::vector<std::future<void>> v_futures;
    auto conc_threads = std::thread::hardware_concurrency();
    conc_threads += add_threads;
    auto interval = block_count / conc_threads;

    for (size_t ith = 0; ith < conc_threads; ++ith)
    {
        v_futures.emplace_back(std::move(std::async(std::launch::async, [=, &func_load]()
                                                    {
                                                        size_t i = ith * interval;
                                                        auto max_count = i + interval;
                                                        if ((ith + 1) == conc_threads)
                                                            max_count = block_count;
                                                        func_load(i, max_count);
                                                    })));
    }
    for (auto &it_future : v_futures)
    {
        it_future.get();
    }
};