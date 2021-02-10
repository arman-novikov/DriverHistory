#pragma once
#include "../DriverHistory.hpp"

#include <initializer_list>
#include <memory>

#include <gtest/gtest.h>

namespace get_sleep_bounds_ns {
timestamp_t TIMEPOINT{1612960200}; // UTC+0 10-02-2021 12:30:00
}

struct GetSleepBounds_data_set_t  {
    timestamp_t ts;
    std::initializer_list<status_t> online_data;
    std::pair<opt_timestamp_t, opt_timestamp_t> expected;
    std::string name;
};
extern GetSleepBounds_data_set_t getSleepBounds_data_set[];

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
    using namespace get_sleep_bounds_ns;
    auto param = GetParam();
    online_data_t d(HISTORY_WINDOW_SIZE);
    d.insert(d.end(), param.online_data.begin(), param.online_data.end());
    dh->Update(d, TIMEPOINT);
    EXPECT_EQ(dh->GetSleepBounds(), param.expected);
}

INSTANTIATE_TEST_CASE_P(Default, GetSleepBoundsTest,
                        testing::Values(getSleepBounds_data_set));
