/**
 * @file log.hpp
 * @brief Bishop logging runtime library.
 *
 * Provides logging operations for Bishop programs.
 * This header is included when programs import the log module.
 *
 * Features:
 * - Thread-safe logging with mutex protection
 * - Multiple log levels (DEBUG, INFO, WARN, ERROR)
 * - Configurable output format
 * - Console and file output support
 * - Per-file log level filtering
 */

#pragma once

#include <bishop/std.hpp>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <vector>
#include <memory>

namespace log {

// Log level constants
constexpr int DEBUG = 0;
constexpr int INFO = 1;
constexpr int WARN = 2;
constexpr int ERROR = 3;

/**
 * Internal logging state.
 * Uses thread-safe access patterns.
 */
struct LogState {
    std::mutex mutex;
    int current_level = INFO;
    std::string format_string = "[%Y-%m-%d %H:%M:%S] [%l] %v";

    struct FileOutput {
        std::unique_ptr<std::ofstream> file;
        int min_level;
    };
    std::vector<FileOutput> file_outputs;
};

/**
 * Gets the global logging state.
 */
inline LogState& get_state() {
    static LogState state;
    return state;
}

/**
 * Thread-safe wrapper for localtime.
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
 * Converts log level to string representation.
 */
inline std::string level_to_string(int level) {
    switch (level) {
        case DEBUG: return "DEBUG";
        case INFO:  return "INFO";
        case WARN:  return "WARN";
        case ERROR: return "ERROR";
        default:    return "UNKNOWN";
    }
}

/**
 * Formats a log message according to the current format string.
 *
 * Supported format specifiers:
 * - %Y, %m, %d, %H, %M, %S - strftime specifiers for timestamp
 * - %l - log level name
 * - %v - the log message
 */
inline std::string format_message(int level, const std::string& message, const std::string& format) {
    auto now = std::chrono::system_clock::now();
    auto time_t_val = std::chrono::system_clock::to_time_t(now);
    std::tm tm_val = safe_localtime(time_t_val);

    std::string result;
    result.reserve(format.size() + message.size() + 32);

    for (size_t i = 0; i < format.size(); ++i) {
        if (format[i] == '%' && i + 1 < format.size()) {
            char spec = format[i + 1];
            ++i;

            switch (spec) {
                case 'Y': {
                    result += std::to_string(tm_val.tm_year + 1900);
                    break;
                }
                case 'm': {
                    if (tm_val.tm_mon + 1 < 10) result += '0';
                    result += std::to_string(tm_val.tm_mon + 1);
                    break;
                }
                case 'd': {
                    if (tm_val.tm_mday < 10) result += '0';
                    result += std::to_string(tm_val.tm_mday);
                    break;
                }
                case 'H': {
                    if (tm_val.tm_hour < 10) result += '0';
                    result += std::to_string(tm_val.tm_hour);
                    break;
                }
                case 'M': {
                    if (tm_val.tm_min < 10) result += '0';
                    result += std::to_string(tm_val.tm_min);
                    break;
                }
                case 'S': {
                    if (tm_val.tm_sec < 10) result += '0';
                    result += std::to_string(tm_val.tm_sec);
                    break;
                }
                case 'l': {
                    result += level_to_string(level);
                    break;
                }
                case 'v': {
                    result += message;
                    break;
                }
                case '%': {
                    result += '%';
                    break;
                }
                default: {
                    result += '%';
                    result += spec;
                    break;
                }
            }
        } else {
            result += format[i];
        }
    }

    return result;
}

/**
 * Internal logging implementation.
 */
inline void log_impl(int level, const std::string& message) {
    auto& state = get_state();
    std::lock_guard<std::mutex> lock(state.mutex);

    // Check if message should be logged to console
    if (level >= state.current_level) {
        std::string formatted = format_message(level, message, state.format_string);
        std::ostream& out = (level >= WARN) ? std::cerr : std::cout;
        out << formatted << std::endl;
    }

    // Write to file outputs
    for (auto& fo : state.file_outputs) {
        if (level >= fo.min_level && fo.file && fo.file->is_open()) {
            std::string formatted = format_message(level, message, state.format_string);
            *fo.file << formatted << std::endl;
            fo.file->flush();
        }
    }
}

/**
 * Internal logging implementation with key-value pair.
 */
inline void log_impl_kv(int level, const std::string& message, const std::string& key, const std::string& value) {
    std::string full_message = message + " {" + key + ": " + value + "}";
    log_impl(level, full_message);
}

/**
 * Logs a debug-level message.
 */
inline void debug(const std::string& message) {
    log_impl(DEBUG, message);
}

/**
 * Logs a debug-level message with a key-value pair.
 */
inline void debug_kv(const std::string& message, const std::string& key, const std::string& value) {
    log_impl_kv(DEBUG, message, key, value);
}

/**
 * Logs an info-level message.
 */
inline void info(const std::string& message) {
    log_impl(INFO, message);
}

/**
 * Logs an info-level message with a key-value pair.
 */
inline void info_kv(const std::string& message, const std::string& key, const std::string& value) {
    log_impl_kv(INFO, message, key, value);
}

/**
 * Logs a warning-level message.
 */
inline void warn(const std::string& message) {
    log_impl(WARN, message);
}

/**
 * Logs a warning-level message with a key-value pair.
 */
inline void warn_kv(const std::string& message, const std::string& key, const std::string& value) {
    log_impl_kv(WARN, message, key, value);
}

/**
 * Logs an error-level message.
 */
inline void error(const std::string& message) {
    log_impl(ERROR, message);
}

/**
 * Logs an error-level message with a key-value pair.
 */
inline void error_kv(const std::string& message, const std::string& key, const std::string& value) {
    log_impl_kv(ERROR, message, key, value);
}

/**
 * Sets the minimum logging level for console output.
 * Messages below this level will be suppressed.
 */
inline void set_level(int level) {
    auto& state = get_state();
    std::lock_guard<std::mutex> lock(state.mutex);
    state.current_level = level;
}

/**
 * Sets the log output format.
 *
 * Supported format specifiers:
 * - %Y - 4-digit year
 * - %m - 2-digit month (01-12)
 * - %d - 2-digit day (01-31)
 * - %H - 2-digit hour (00-23)
 * - %M - 2-digit minute (00-59)
 * - %S - 2-digit second (00-59)
 * - %l - log level name
 * - %v - the log message
 * - %% - literal percent sign
 */
inline void set_format(const std::string& format) {
    auto& state = get_state();
    std::lock_guard<std::mutex> lock(state.mutex);
    state.format_string = format;
}

/**
 * Adds a file as a logging output destination.
 * All log messages at or above the current level will be written to this file.
 */
inline void add_file(const std::string& path) {
    auto& state = get_state();
    std::lock_guard<std::mutex> lock(state.mutex);

    LogState::FileOutput fo;
    fo.file = std::make_unique<std::ofstream>(path, std::ios::app);
    fo.min_level = state.current_level;
    state.file_outputs.push_back(std::move(fo));
}

/**
 * Adds a file as a logging output destination with a specific minimum level.
 * Only messages at or above the specified level will be written to this file.
 */
inline void add_file_level(const std::string& path, int level) {
    auto& state = get_state();
    std::lock_guard<std::mutex> lock(state.mutex);

    LogState::FileOutput fo;
    fo.file = std::make_unique<std::ofstream>(path, std::ios::app);
    fo.min_level = level;
    state.file_outputs.push_back(std::move(fo));
}

}  // namespace log
