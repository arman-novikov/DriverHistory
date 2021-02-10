#include <iostream>
#include <algorithm>
#include <iomanip>
#include <functional>

#include "DriverHistory.hpp"
#include <gtest/gtest.h>

std::initializer_list<status_t> d1 {
    //0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1
    //0, 1, 1, 1, 1, 1, 1
    //0, 0, 1, 1, 1, 1, 1, 1, 0
    1, 1, 1, 1, 1, 1, 1, 1, 1,
};

std::initializer_list<status_t> d2 {
    // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1
};

timestamp_t floor(const std::chrono::time_point<std::chrono::system_clock>& tp, size_t rounding_seconds)
{
    timestamp_t res = std::chrono::system_clock::to_time_t(tp);
    res -= res % rounding_seconds;
    return res;

}

void print(const timestamp_t& tp) {
    std::cout << std::put_time(std::localtime(&tp), "%F %T") << '\n';
}

int main(int argc, char* argv[])
{
    using namespace std::chrono_literals;
    ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
}
