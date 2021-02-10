#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include <gtest/gtest.h>

#include "../DriverHistory.hpp"

struct GetSleepBounds_data_set_t  {
    GetSleepBounds_data_set_t(
        timestamp_t ts,
        std::vector<status_t> online_data,
        std::pair<opt_timestamp_t, opt_timestamp_t> expected,
        std::string name):
        ts(ts), online_data(online_data), expected(expected), name(name)
    {}
    GetSleepBounds_data_set_t():
        ts{}, online_data{}, expected{}, name{}
    {}
    friend std::ostream& operator<<(std::ostream& os,
                                    const GetSleepBounds_data_set_t& obj)
    {
        return os << obj.name << std::endl;
    }
    timestamp_t ts;
    std::vector<status_t> online_data;
    std::pair<opt_timestamp_t, opt_timestamp_t> expected;
    std::string name;
};
/*
namespace get_sleep_bounds_ns {
    extern timestamp_t TIMEPOINT;
    extern std::vector<GetSleepBounds_data_set_t> getSleepBounds_data_set;
}
*/
namespace {
auto OF = OFFLINE;
auto ON = ONLINE;
}

timestamp_t TIMEPOINT{1612960200}; // UTC+0 10-02-2021 12:30:00

std::vector <GetSleepBounds_data_set_t>
getSleepBounds_data_set {
    {TIMEPOINT, {}, {}, "empty data"},
    {TIMEPOINT, {OF, OF, OF, ON}, {}, "not sufficient data"},

    {TIMEPOINT, {
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 4h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 8h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, ON, ON, ON, ON, // 12h
        },
    {TIMEPOINT - SMALL_INTERIM * (4 + SLEEP), TIMEPOINT - SMALL_INTERIM * 4},
    "4 interims ago finished to sleep SLEEP hours"},

    {TIMEPOINT, {
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 4h
            ON, ON, ON, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 8h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, ON, // 12h
        },
    {TIMEPOINT - SMALL_INTERIM * (1 + SLEEP), TIMEPOINT - SMALL_INTERIM * 1},
    "1 interim ago finished to sleep SLEEP hours"},
};

struct GetSleepBoundsFixture: testing::Test
{
    std::unique_ptr<DriverHistory> dh;
    GetSleepBoundsFixture() {
        dh = std::make_unique<DriverHistory>();
    }
};

struct GetSleepBoundsTest:
        GetSleepBoundsFixture,
        testing::WithParamInterface<GetSleepBounds_data_set_t>
{};

TEST_P(GetSleepBoundsTest, GetSleepBounds) {
    //using namespace get_sleep_bounds_ns;
    auto param = GetParam();
    online_data_t d(HISTORY_WINDOW_SIZE);
    d.insert(d.end(), param.online_data.begin(), param.online_data.end());
    dh->Update(d, TIMEPOINT);
    //auto [begin, end] = dh->GetSleepBounds();
    EXPECT_EQ(dh->GetSleepBounds(), param.expected);
}

INSTANTIATE_TEST_CASE_P(Default, GetSleepBoundsTest,
                        testing::ValuesIn(
                            getSleepBounds_data_set)
                        );
