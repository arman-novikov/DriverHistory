#include "DriverHistory.hpp"

DriverHistory::DriverHistory():
    onlineData_(online_data_t(HISTORY_WINDOW_SIZE)),
    onOrder_{0}
{

}

std::pair<opt_timestamp_t, opt_timestamp_t>
DriverHistory::GetSleepBounds() const
{
    if (onlineData_.size() < SLEEP) {
        return {std::nullopt, std::nullopt}; // better to throw an exception "not enough data"
    }
    auto it = onlineData_.rbegin();
    auto end = onlineData_.rend() - SLEEP;

    while (it != end) {
        size_t occurency_count = std::count(it,
                                            it + SLEEP,
                                            OFFLINE);
        if (occurency_count == SLEEP) {
            opt_timestamp_t sleep_end = std::nullopt;
            if (it != onlineData_.rbegin()) {
                sleep_end = Interim2Timestamp(
                    timestamp_, it - onlineData_.rbegin()
                );
            }
            it += SLEEP;
            while(*it == OFFLINE && it != onlineData_.rend()){
                ++it;
            }

            if (it == onlineData_.rend()) {
                return {std::nullopt, sleep_end};
            }
            timestamp_t sleep_begin = Interim2Timestamp(
                timestamp_, it - onlineData_.rbegin()
            );
            return {sleep_begin, sleep_end};
        }
        ++it;
    }
    return {std::nullopt, std::nullopt};
}

std::vector<timestamp_t>
DriverHistory::ProcessWindow(std::vector<status_t>& data, timestamp_t ts)
{
    // assert not empty
    size_t offset = 0;
    auto start = data.rbegin();
    auto end = data.rend(); // - SLEEP?

    while(start != end) {
        size_t occurency_count = std::count(start,
                                            start + SLEEP,
                                            OFFLINE);
        if (occurency_count == SLEEP) {
            return {Interim2Timestamp(timestamp_, offset)};

        }
        ++start;
        ++offset;
    }

}

/*
 * todo: use a templated online_data ?
*/
void DriverHistory::Append(const online_data_t& online_data,
            const timestamp_t& ts)
{
    if (onlineData_.empty()) {
        onlineData_.insert(onlineData_.begin(),
                           online_data.begin(), online_data.end());
        timestamp_ = ts;
        return;
    }

    timestamp_t time_diff = ts - timestamp_;
    if (time_diff < 0)
        throw std::runtime_error("timestamp is outdated");

    if (time_diff == 0) {
        return; // no new data
    }

    size_t interim_diff = time_diff / SMALL_INTERIM;

    // todo: extract into a method patch_missing_data()
    // or throw an exception?
    if (online_data.size() < interim_diff) {
        // means some data is missing
        // gonna patch it with default values: OFFLINE
        size_t patch_count = interim_diff - online_data.size();
        onlineData_.insert(onlineData_.end(), patch_count, OFFLINE);
        interim_diff -= patch_count;
    }

    onlineData_.insert(onlineData_.end(),
                       online_data.end() - interim_diff,
                       online_data.end());
    timestamp_ = ts;
}

opt_timestamp_t DriverHistory::GetWorkStart() const
{
    size_t res = 0;
    auto start = onlineData_.rbegin();
    auto end = onlineData_.rend();
    if (onlineData_.empty()) {
        return std::nullopt;
    }

    while(start != end) {
        size_t occurency_count = std::count(start,
                                            start + SLEEP,
                                            OFFLINE);
        if (occurency_count == SLEEP) {
            return Interim2Timestamp(timestamp_, res);
        }
        ++start;
        ++res;
    }

    for (res = 0; res < onlineData_.size(); ++res) {
        if(onlineData_[res] == ONLINE) {
            break;
        }
    }

    if (res == onlineData_.size()) {
        // iterated to the end and no ONLINE mark is found
        return std::nullopt;
    }

    res = onlineData_.size() - res - 1; // -1 since we need a reverse index (from end)
    return Interim2Timestamp(timestamp_, res);
}

/*!
 * @return count of online statuses from work start
 */
DriverHistory::online_review_t DriverHistory::GetOnline() const
{
    auto timestamp_t2interims = [] (timestamp_t from, timestamp_t to)
            -> size_t
    {
        return to - from / SMALL_INTERIM;
    };

    auto from_ts = GetWorkStart();
    if (from_ts == std::nullopt) {
        return {0U, HISTORY_WINDOW_SIZE};
    }

    size_t offset = timestamp_t2interims(from_ts.value(), timestamp_);
    size_t online_interims = std::count(
                onlineData_.end() - offset, onlineData_.end(),
                ONLINE);

    size_t request_offset = online_interims > MAX_ONLINE ? 1:
        MAX_ONLINE - online_interims;

    return {online_interims, request_offset};
}

void DriverHistory::SetOnOrder(size_t on_order)
{
    onOrder_ = on_order;
}

DriverHistory::on_order_review_t DriverHistory::GetOnOrder() const
{
    return {onOrder_, GetWorkStart()};
}

timestamp_t DriverHistory::Interim2Timestamp(timestamp_t ts, size_t reverse_offset)
{
    return ts - reverse_offset * SMALL_INTERIM;
}





















