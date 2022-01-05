#include <iostream>
#include "threads-utils.hpp"
#include "lf-stack.hpp"

int main(int, char**) {
    lockFreeStack<size_t> lfStack;
    std::atomic<size_t> res = 0;

    auto l_func_load = [&](size_t i = 0, size_t max = 0) {
        std::cout << "func load " << i << " " << max << std::endl;
        size_t result = 0;
        size_t temp_res = 0;
        for (; i < max; i++)
        {
            lfStack.push(1);
            if (i % 2 == 0) {
                lfStack.pop(temp_res);
                result += temp_res;
            }
        }
        res += result;
    };

    l_thread_data<decltype(l_func_load)>(l_func_load, 16000000);
    std::cout << "Result " << res << std::endl;
    

}
