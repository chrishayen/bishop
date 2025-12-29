/**
 * @file time.hpp
 * @brief Bishop time runtime library.
 *
 * Provides time and duration operations for Bishop programs.
 * Uses std::chrono for all time operations.
 */

#pragma once

#include <bishop/std.hpp>
#include <bishop/error.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace bishop_time {

/**
 * Thread-safe wrapper for localtime.
 * Uses localtime_r on POSIX systems.
 */
inline std::tm safe_localtime(std::time_t time) {
    std::tm result = {};
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&result, &time);
#else
    localtime_r(&time, &result);
#endif
    return result;
}

/**
 * Thread-safe wrapper for gmtime.
 * Uses gmtime_r on POSIX systems.
 */
inline std::tm safe_gmtime(std::time_t time) {
    std::tm result = {};
#if defined(_WIN32) || defined(_WIN64)
    gmtime_s(&result, &time);
#else
    gmtime_r(&time, &result);
#endif
    return result;
}

/**
 * Duration represents a time span in milliseconds.
 * Inspired by std::chrono::duration, providing type-safe time units.
 */
struct Duration {
    int64_t millis_value;

    /**
     * Returns the duration in milliseconds.
     */
    int64_t as_millis() const {
        return millis_value;
    }

    /**
     * Returns the duration in seconds.
     */
    int64_t as_seconds() const {
        return millis_value / 1000;
    }

    /**
     * Returns the duration in minutes.
     */
    int64_t as_minutes() const {
        return millis_value / (1000 * 60);
    }

    /**
     * Returns the duration in hours.
     */
    int64_t as_hours() const {
        return millis_value / (1000 * 60 * 60);
    }

    /**
     * Returns the duration in days.
     */
    int64_t as_days() const {
        return millis_value / (1000 * 60 * 60 * 24);
    }

    /**
     * Adds two durations.
     */
    Duration operator+(const Duration& other) const {
        return Duration{millis_value + other.millis_value};
    }

    /**
     * Subtracts two durations.
     */
    Duration operator-(const Duration& other) const {
        return Duration{millis_value - other.millis_value};
    }

    /**
     * Compares two durations for equality.
     */
    bool operator==(const Duration& other) const {
        return millis_value == other.millis_value;
    }

    /**
     * Compares two durations for inequality.
     */
    bool operator!=(const Duration& other) const {
        return millis_value != other.millis_value;
    }

    /**
     * Less than comparison.
     */
    bool operator<(const Duration& other) const {
        return millis_value < other.millis_value;
    }

    /**
     * Less than or equal comparison.
     */
    bool operator<=(const Duration& other) const {
        return millis_value <= other.millis_value;
    }

    /**
     * Greater than comparison.
     */
    bool operator>(const Duration& other) const {
        return millis_value > other.millis_value;
    }

    /**
     * Greater than or equal comparison.
     */
    bool operator>=(const Duration& other) const {
        return millis_value >= other.millis_value;
    }
};

/**
 * Timestamp represents a point in time.
 * Contains broken-down time components for easy access.
 */
struct Timestamp {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int millisecond;
    int weekday;  // 0 = Sunday, 1 = Monday, ..., 6 = Saturday

    // Internal: stores the time point for arithmetic
    std::chrono::system_clock::time_point time_point;

    /**
     * Returns Unix timestamp in seconds.
     */
    int64_t unix() const {
        auto duration = time_point.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    }

    /**
     * Returns Unix timestamp in milliseconds.
     */
    int64_t unix_millis() const {
        auto duration = time_point.time_since_epoch();
        return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    }

    /**
     * Formats the timestamp using strftime format specifiers.
     * Note: Always formats in local timezone.
     */
    std::string format(const std::string& fmt) const {
        auto time_t_val = std::chrono::system_clock::to_time_t(time_point);
        std::tm tm_val = safe_localtime(time_t_val);

        std::ostringstream oss;
        oss << std::put_time(&tm_val, fmt.c_str());
        return oss.str();
    }

    /**
     * Adds a duration to this timestamp.
     */
    Timestamp operator+(const Duration& d) const {
        auto new_tp = time_point + std::chrono::milliseconds(d.millis_value);
        return from_time_point(new_tp);
    }

    /**
     * Subtracts a duration from this timestamp.
     */
    Timestamp operator-(const Duration& d) const {
        auto new_tp = time_point - std::chrono::milliseconds(d.millis_value);
        return from_time_point(new_tp);
    }

    /**
     * Returns the duration between two timestamps.
     */
    Duration operator-(const Timestamp& other) const {
        auto diff = time_point - other.time_point;
        int64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
        return Duration{ms};
    }

    /**
     * Compares two timestamps for equality.
     */
    bool operator==(const Timestamp& other) const {
        return time_point == other.time_point;
    }

    /**
     * Compares two timestamps for inequality.
     */
    bool operator!=(const Timestamp& other) const {
        return time_point != other.time_point;
    }

    /**
     * Less than comparison.
     */
    bool operator<(const Timestamp& other) const {
        return time_point < other.time_point;
    }

    /**
     * Less than or equal comparison.
     */
    bool operator<=(const Timestamp& other) const {
        return time_point <= other.time_point;
    }

    /**
     * Greater than comparison.
     */
    bool operator>(const Timestamp& other) const {
        return time_point > other.time_point;
    }

    /**
     * Greater than or equal comparison.
     */
    bool operator>=(const Timestamp& other) const {
        return time_point >= other.time_point;
    }

    /**
     * Creates a Timestamp from a chrono time_point.
     */
    static Timestamp from_time_point(std::chrono::system_clock::time_point tp) {
        Timestamp ts;
        ts.time_point = tp;

        auto time_t_val = std::chrono::system_clock::to_time_t(tp);
        std::tm tm_val = safe_localtime(time_t_val);

        ts.year = tm_val.tm_year + 1900;
        ts.month = tm_val.tm_mon + 1;
        ts.day = tm_val.tm_mday;
        ts.hour = tm_val.tm_hour;
        ts.minute = tm_val.tm_min;
        ts.second = tm_val.tm_sec;
        ts.weekday = tm_val.tm_wday;

        // Extract milliseconds from time_point
        auto duration = tp.time_since_epoch();
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration - secs);
        ts.millisecond = static_cast<int>(ms.count());

        return ts;
    }

    /**
     * Creates a Timestamp from a chrono time_point in UTC.
     */
    static Timestamp from_time_point_utc(std::chrono::system_clock::time_point tp) {
        Timestamp ts;
        ts.time_point = tp;

        auto time_t_val = std::chrono::system_clock::to_time_t(tp);
        std::tm tm_val = safe_gmtime(time_t_val);

        ts.year = tm_val.tm_year + 1900;
        ts.month = tm_val.tm_mon + 1;
        ts.day = tm_val.tm_mday;
        ts.hour = tm_val.tm_hour;
        ts.minute = tm_val.tm_min;
        ts.second = tm_val.tm_sec;
        ts.weekday = tm_val.tm_wday;

        // Extract milliseconds from time_point
        auto duration = tp.time_since_epoch();
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration - secs);
        ts.millisecond = static_cast<int>(ms.count());

        return ts;
    }
};

/**
 * Creates a Duration of the specified milliseconds.
 */
inline Duration millis(int64_t ms) {
    return Duration{ms};
}

/**
 * Creates a Duration of the specified seconds.
 */
inline Duration seconds(int64_t s) {
    return Duration{s * 1000};
}

/**
 * Creates a Duration of the specified minutes.
 */
inline Duration minutes(int64_t m) {
    return Duration{m * 60 * 1000};
}

/**
 * Creates a Duration of the specified hours.
 */
inline Duration hours(int64_t h) {
    return Duration{h * 60 * 60 * 1000};
}

/**
 * Creates a Duration of the specified days.
 */
inline Duration days(int64_t d) {
    return Duration{d * 24 * 60 * 60 * 1000};
}

/**
 * Returns the current local time.
 */
inline Timestamp now() {
    return Timestamp::from_time_point(std::chrono::system_clock::now());
}

/**
 * Returns the current UTC time.
 */
inline Timestamp now_utc() {
    return Timestamp::from_time_point_utc(std::chrono::system_clock::now());
}

/**
 * Returns the elapsed duration since the given timestamp.
 */
inline Duration since(const Timestamp& ts) {
    auto now_tp = std::chrono::system_clock::now();
    auto diff = now_tp - ts.time_point;
    int64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    return Duration{ms};
}

/**
 * Parses a timestamp from a string using strptime format specifiers.
 */
inline bishop::rt::Result<Timestamp> parse(const std::string& str, const std::string& fmt) {
    std::tm tm_val = {};
    std::istringstream iss(str);
    iss >> std::get_time(&tm_val, fmt.c_str());

    if (iss.fail()) {
        return std::make_shared<bishop::rt::Error>(
            "Failed to parse time: '" + str + "' with format '" + fmt + "'");
    }

    // Set default values for unspecified fields
    if (tm_val.tm_mday == 0) {
        tm_val.tm_mday = 1;
    }

    auto time_t_val = std::mktime(&tm_val);

    if (time_t_val == -1) {
        return std::make_shared<bishop::rt::Error>(
            "Failed to convert parsed time to timestamp");
    }

    auto tp = std::chrono::system_clock::from_time_t(time_t_val);
    return Timestamp::from_time_point(tp);
}

}  // namespace bishop_time
