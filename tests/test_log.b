// Tests for the log module

import log;

fn test_log_debug() {
    // Should not crash - debug level logging
    log.debug("Debug message");
}

fn test_log_info() {
    // Should not crash - info level logging
    log.info("Info message");
}

fn test_log_warn() {
    // Should not crash - warn level logging
    log.warn("Warning message");
}

fn test_log_error() {
    // Should not crash - error level logging
    log.error("Error message");
}

fn test_log_with_values() {
    // Should not crash - logging with key-value pairs
    log.info_kv("User logged in", "user_id", "12345");
    log.debug_kv("Request received", "method", "GET");
    log.warn_kv("High memory usage", "percent", "95");
    log.error_kv("Connection failed", "host", "example.com");
}

fn test_set_level_debug() {
    // Set level to debug - all messages should be logged
    log.set_level(log.DEBUG);
    log.debug("Should appear at DEBUG level");
    log.info("Should appear at DEBUG level");
}

fn test_set_level_info() {
    // Set level to info - debug should be suppressed
    log.set_level(log.INFO);
    log.debug("Should be suppressed");
    log.info("Should appear at INFO level");
}

fn test_set_level_warn() {
    // Set level to warn - debug and info should be suppressed
    log.set_level(log.WARN);
    log.debug("Should be suppressed");
    log.info("Should be suppressed");
    log.warn("Should appear at WARN level");
}

fn test_set_level_error() {
    // Set level to error - only errors should appear
    log.set_level(log.ERROR);
    log.debug("Should be suppressed");
    log.info("Should be suppressed");
    log.warn("Should be suppressed");
    log.error("Should appear at ERROR level");
}

fn test_set_format() {
    // Set custom format string
    log.set_format("[%Y-%m-%d %H:%M:%S] [%l] %v");
    log.info("Message with custom format");
}

fn test_add_file() {
    // Add file logging output
    log.add_file("/tmp/bishop_test.log");
    log.info("This should go to file");
}

fn test_add_file_with_level() {
    // Add file logging with specific level
    log.add_file_level("/tmp/bishop_error.log", log.ERROR);
    log.info("This should NOT go to error log");
    log.error("This SHOULD go to error log");
}

fn test_level_constants() {
    // Verify level constants exist and have expected ordering
    assert_eq(true, log.DEBUG < log.INFO);
    assert_eq(true, log.INFO < log.WARN);
    assert_eq(true, log.WARN < log.ERROR);
}
