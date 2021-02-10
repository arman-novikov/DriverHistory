#include <iostream>
#include <algorithm>
#include <iomanip>
#include <functional>

#include "DriverHistory.hpp"

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
    std::cout << tp << " : " << std::put_time(std::localtime(&tp), "%F %T") << '\n';
}

int main(int, char*[])
{
    using namespace std::chrono_literals;

    auto flr = std::bind(floor, std::placeholders::_1, SMALL_INTERIM);
    DriverHistory dd;
    auto onl = online_data_t(HISTORY_WINDOW_SIZE);

    onl.insert(onl.begin(), d1.begin(), d1.end());
    auto now = std::chrono::system_clock::now();
    print(flr(now));
    dd.Update(onl, flr(now));

    onl.clear();
    onl.insert(onl.begin(), d2.begin(), d2.end());
    now += 2h;
    dd.Update(onl, flr(now));
    print(flr(now));

    for (auto i: dd.onlineData_) {
        std::cout << static_cast<int>(i) << " ";
    }
    std::cout << std::endl;
    return 0;
}
