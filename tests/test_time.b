/// Tests for the time module

import time;

// ============================================================
// Tests for Duration construction
// ============================================================

fn test_duration_millis() {
    d := time.millis(5000);
    assert_eq(5000, d.as_millis());
}

fn test_duration_seconds() {
    d := time.seconds(90);
    assert_eq(90, d.as_seconds());
    assert_eq(90000, d.as_millis());
}

fn test_duration_minutes() {
    d := time.minutes(5);
    assert_eq(5, d.as_minutes());
    assert_eq(300, d.as_seconds());
    assert_eq(300000, d.as_millis());
}

fn test_duration_hours() {
    d := time.hours(2);
    assert_eq(2, d.as_hours());
    assert_eq(120, d.as_minutes());
    assert_eq(7200, d.as_seconds());
}

fn test_duration_days() {
    d := time.days(1);
    assert_eq(1, d.as_days());
    assert_eq(24, d.as_hours());
    assert_eq(1440, d.as_minutes());
    assert_eq(86400, d.as_seconds());
}

// ============================================================
// Tests for Duration arithmetic
// ============================================================

fn test_duration_add() {
    d1 := time.hours(1);
    d2 := time.minutes(30);
    total := d1 + d2;
    assert_eq(90, total.as_minutes());
}

fn test_duration_subtract() {
    d1 := time.hours(2);
    d2 := time.minutes(30);
    diff := d1 - d2;
    assert_eq(90, diff.as_minutes());
}

// ============================================================
// Tests for Timestamp construction
// ============================================================

fn test_timestamp_now() {
    now := time.now();
    assert_eq(true, now.year >= 2024);
    assert_eq(true, now.month >= 1);
    assert_eq(true, now.month <= 12);
    assert_eq(true, now.day >= 1);
    assert_eq(true, now.day <= 31);
    assert_eq(true, now.hour >= 0);
    assert_eq(true, now.hour <= 23);
    assert_eq(true, now.minute >= 0);
    assert_eq(true, now.minute <= 59);
    assert_eq(true, now.second >= 0);
    assert_eq(true, now.second <= 59);
}

fn test_timestamp_now_utc() {
    now := time.now_utc();
    assert_eq(true, now.year >= 2024);
    assert_eq(true, now.month >= 1);
    assert_eq(true, now.month <= 12);
}

fn test_timestamp_unix() {
    now := time.now();
    unix := now.unix();
    assert_eq(true, unix > 1700000000);
}

fn test_timestamp_unix_millis() {
    now := time.now();
    unix_ms := now.unix_millis();
    assert_eq(true, unix_ms > 1700000000000);
}

fn test_timestamp_weekday() {
    now := time.now();
    assert_eq(true, now.weekday >= 0);
    assert_eq(true, now.weekday <= 6);
}

fn test_timestamp_millisecond() {
    now := time.now();
    assert_eq(true, now.millisecond >= 0);
    assert_eq(true, now.millisecond <= 999);
}

// ============================================================
// Tests for Timestamp arithmetic
// ============================================================

fn test_timestamp_add_duration() {
    now := time.now();
    future := now + time.days(7);
    diff := future - now;
    assert_eq(7, diff.as_days());
}

fn test_timestamp_subtract_duration() {
    now := time.now();
    past := now - time.hours(2);
    diff := now - past;
    assert_eq(2, diff.as_hours());
}

fn test_timestamp_subtract_timestamp() {
    now := time.now();
    future := now + time.minutes(30);
    diff := future - now;
    assert_eq(30, diff.as_minutes());
}

// ============================================================
// Tests for time.since
// ============================================================

fn test_time_since() {
    start := time.now();
    sleep(10);
    elapsed := time.since(start);
    assert_eq(true, elapsed.as_millis() >= 10);
}

// ============================================================
// Tests for formatting and parsing
// ============================================================

fn test_timestamp_format() -> void or err {
    ts := time.parse("2024-01-15 14:30:00", "%Y-%m-%d %H:%M:%S") or fail err;
    formatted := ts.format("%Y-%m-%d");
    assert_eq("2024-01-15", formatted);
}

fn test_timestamp_format_time() -> void or err {
    ts := time.parse("2024-01-15 14:30:45", "%Y-%m-%d %H:%M:%S") or fail err;
    formatted := ts.format("%H:%M:%S");
    assert_eq("14:30:45", formatted);
}

fn test_time_parse_basic() -> void or err {
    ts := time.parse("2024-01-15", "%Y-%m-%d") or fail err;
    assert_eq(2024, ts.year);
    assert_eq(1, ts.month);
    assert_eq(15, ts.day);
}

fn test_time_parse_full() -> void or err {
    ts := time.parse("2024-03-20 10:15:30", "%Y-%m-%d %H:%M:%S") or fail err;
    assert_eq(2024, ts.year);
    assert_eq(3, ts.month);
    assert_eq(20, ts.day);
    assert_eq(10, ts.hour);
    assert_eq(15, ts.minute);
    assert_eq(30, ts.second);
}

fn test_time_parse_invalid_fails() -> void or err {
    ts := time.parse("not-a-date", "%Y-%m-%d") or {
        return;
    };

    assert_eq(true, false);
}

// ============================================================
// Tests for Duration comparison operators
// ============================================================

fn test_duration_equality() {
    d1 := time.seconds(60);
    d2 := time.minutes(1);
    d3 := time.seconds(30);

    assert_eq(true, d1 == d2);
    assert_eq(false, d1 == d3);
    assert_eq(true, d1 != d3);
    assert_eq(false, d1 != d2);
}

fn test_duration_less_than() {
    d1 := time.seconds(30);
    d2 := time.minutes(1);

    assert_eq(true, d1 < d2);
    assert_eq(false, d2 < d1);
    assert_eq(false, d1 < d1);
}

fn test_duration_less_than_or_equal() {
    d1 := time.seconds(30);
    d2 := time.minutes(1);
    d3 := time.seconds(60);

    assert_eq(true, d1 <= d2);
    assert_eq(true, d2 <= d3);
    assert_eq(false, d2 <= d1);
}

fn test_duration_greater_than() {
    d1 := time.hours(1);
    d2 := time.minutes(30);

    assert_eq(true, d1 > d2);
    assert_eq(false, d2 > d1);
    assert_eq(false, d1 > d1);
}

fn test_duration_greater_than_or_equal() {
    d1 := time.hours(1);
    d2 := time.minutes(60);
    d3 := time.minutes(30);

    assert_eq(true, d1 >= d2);
    assert_eq(true, d1 >= d3);
    assert_eq(false, d3 >= d1);
}

// ============================================================
// Tests for Timestamp comparison operators
// ============================================================

fn test_timestamp_equality() {
    now := time.now();
    future := now + time.seconds(1);

    assert_eq(false, now == future);
    assert_eq(true, now != future);
}

fn test_timestamp_less_than() {
    now := time.now();
    future := now + time.hours(1);

    assert_eq(true, now < future);
    assert_eq(false, future < now);
}

fn test_timestamp_greater_than() {
    now := time.now();
    past := now - time.hours(1);

    assert_eq(true, now > past);
    assert_eq(false, past > now);
}

fn test_timestamp_less_than_or_equal() {
    now := time.now();
    future := now + time.hours(1);

    assert_eq(true, now <= future);
    assert_eq(true, now <= now);
    assert_eq(false, future <= now);
}

fn test_timestamp_greater_than_or_equal() {
    now := time.now();
    past := now - time.hours(1);

    assert_eq(true, now >= past);
    assert_eq(true, now >= now);
    assert_eq(false, past >= now);
}

// ============================================================
// Tests for edge cases
// ============================================================

fn test_duration_zero() {
    d := time.millis(0);
    assert_eq(0, d.as_millis());
    assert_eq(0, d.as_seconds());
    assert_eq(0, d.as_minutes());
    assert_eq(0, d.as_hours());
    assert_eq(0, d.as_days());
}

fn test_duration_negative() {
    d := time.seconds(-60);
    assert_eq(-60, d.as_seconds());
    assert_eq(-60000, d.as_millis());
}

fn test_duration_negative_comparison() {
    d1 := time.seconds(-60);
    d2 := time.seconds(0);
    d3 := time.seconds(60);

    assert_eq(true, d1 < d2);
    assert_eq(true, d1 < d3);
    assert_eq(true, d2 < d3);
}

fn test_duration_large_values() {
    d := time.days(365);
    assert_eq(365, d.as_days());
    assert_eq(8760, d.as_hours());
}

// ============================================================
// Tests for measuring elapsed time
// ============================================================

fn test_measure_elapsed() {
    start := time.now();
    sleep(50);
    elapsed := time.now() - start;
    assert_eq(true, elapsed.as_millis() >= 50);
}
