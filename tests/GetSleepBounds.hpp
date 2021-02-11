#pragma once

#include "common.hpp"

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

    {TIMEPOINT, {
            ON, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 4h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, ON, ON, ON, // 8h
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 12h
        },
    {TIMEPOINT - SMALL_INTERIM * 47, TIMEPOINT - SMALL_INTERIM * (47 - SLEEP)},
    "1 interim ONLINE and start sleeping for SLEEP hours"},

    {TIMEPOINT, {
            ON, ON, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 4h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, ON, ON, // 8h
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 12h
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 16h
        },
    {TIMEPOINT - SMALL_INTERIM * 62, TIMEPOINT - SMALL_INTERIM * (62 - SLEEP)},
    "2 interims ONLINE and start sleeping for SLEEP hours"},

    {TIMEPOINT, {
            ON, ON, ON, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 4h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, ON, ON, // 8h
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 12h
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 16h
        },
        {std::nullopt, std::nullopt},
    "1 OFFLINE interim is not sufficient for SLEEP hours"},

    {TIMEPOINT, {
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 4h
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 8h
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 12h
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 16h
        },
        {std::nullopt, std::nullopt},
    "permanently online"},

    {TIMEPOINT, {
            ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, // 4h
            ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, // 8h
            ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, // 12h
            ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, // 16h
            ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, ON, OF, // 20h
        },
        {std::nullopt, std::nullopt},
    "shuttling online-offline"},

    {TIMEPOINT, {
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 4h
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 8h
            ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, // 12h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 16h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 20h
        },
        {TIMEPOINT - 32 * SMALL_INTERIM, std::nullopt},
    "has been offline for at least 8 hours"},

    {TIMEPOINT, {
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 4h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 8h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 12h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, ON, // 16h
        },
        {std::nullopt, TIMEPOINT - SMALL_INTERIM},
    "has been offline untill the last interim"},

    {TIMEPOINT, {
            ON, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 4h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 8h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 12h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 16h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, ON, // 20h
        },
        {TIMEPOINT - 79 * SMALL_INTERIM, TIMEPOINT - SMALL_INTERIM},
    "bordering ONLINEs"},

    {TIMEPOINT, {
            ON, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 4h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 8h
            OF, OF, OF, ON, ON, OF, ON, OF, OF, OF, OF, OF, ON, OF, OF, ON, // 12h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, // 16h
            OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, ON, // 20h
        },
        {TIMEPOINT - 32 * SMALL_INTERIM, TIMEPOINT - SMALL_INTERIM},
    "several sleeps"},


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
    auto param = GetParam();
    online_data_t d(HISTORY_WINDOW_SIZE);
    d.insert(d.end(), param.online_data.begin(), param.online_data.end());
    dh->Append(d, TIMEPOINT);
    EXPECT_EQ(dh->GetSleepBounds(), param.expected);
}

INSTANTIATE_TEST_SUITE_P(
    Default,
    GetSleepBoundsTest,
    testing::ValuesIn(getSleepBounds_data_set)
);
