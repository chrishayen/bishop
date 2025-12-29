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

fn test_timestamp_format() or err {
    ts := time.parse("2024-01-15 14:30:00", "%Y-%m-%d %H:%M:%S") or fail err;
    formatted := ts.format("%Y-%m-%d");
    assert_eq("2024-01-15", formatted);
}

fn test_timestamp_format_time() or err {
    ts := time.parse("2024-01-15 14:30:45", "%Y-%m-%d %H:%M:%S") or fail err;
    formatted := ts.format("%H:%M:%S");
    assert_eq("14:30:45", formatted);
}

fn test_time_parse_basic() or err {
    ts := time.parse("2024-01-15", "%Y-%m-%d") or fail err;
    assert_eq(2024, ts.year);
    assert_eq(1, ts.month);
    assert_eq(15, ts.day);
}

fn test_time_parse_full() or err {
    ts := time.parse("2024-03-20 10:15:30", "%Y-%m-%d %H:%M:%S") or fail err;
    assert_eq(2024, ts.year);
    assert_eq(3, ts.month);
    assert_eq(20, ts.day);
    assert_eq(10, ts.hour);
    assert_eq(15, ts.minute);
    assert_eq(30, ts.second);
}

fn test_time_parse_invalid_fails() or err {
    ts := time.parse("not-a-date", "%Y-%m-%d") or {
        return;
    };

    assert_eq(true, false);
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
