#include "test_GetSleepBounds.hpp"

namespace {
auto OF = OFFLINE;
auto ON = ONLINE;
}
using namespace get_sleep_bounds_ns;

struct GetSleepBounds_data_set_t getSleepBounds_data_set[] {
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
