#pragma once

#include <future>
#include <iostream>
#include <vector>

template <typename TLoadFunc>
auto l_thread_data = [](TLoadFunc func_load, size_t block_count = 1, size_t add_threads = 0)
{
    std::vector<std::future<void>> v_futures;
    std::mutex m_cout;
    /// create several transactions
    auto conc_threads = std::thread::hardware_concurrency();
    conc_threads += add_threads;
    // std::cout << "Real thread count: " << conc_threads << std::endl;
    auto interval = block_count / conc_threads;
    for (size_t ith = 0; ith < conc_threads; ++ith)
    {
        v_futures.emplace_back(std::move(std::async(std::launch::async, [=, &m_cout, &func_load]()
                                                    {
                                                        size_t i = ith * interval;
                                                        auto max_count = i + interval;
                                                        if ((ith + 1) == conc_threads)
                                                            max_count = block_count;
                                                        // {
                                                        //     std::lock_guard<std::mutex> lock(m_cout);
                                                        //     std::cout << "Iter threads: " << ith << " Interval start: " << i << " Interval end: " << max_count << std::endl;
                                                        // }
                                                        func_load(i, max_count);
                                                    })));
    }
    // {
    //     std::lock_guard<std::mutex> lock(m_cout);
    //     std::cout << "Wait futures" << std::endl;
    // }
    for (auto &it_future : v_futures)
    {
        it_future.get();
    }
};