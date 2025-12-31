/**
 * @file time.cpp
 * @brief Built-in time module implementation.
 *
 * Creates the AST definitions for the time module.
 * The actual runtime is in runtime/time/time.hpp and included as a header.
 */

/**
 * @nog_struct Duration
 * @module time
 * @description Represents a time span. Stores internally as milliseconds.
 * @field millis_value int - The duration in milliseconds (internal)
 * @example
 * d := time.seconds(90);
 * print(d.as_seconds());  // 90
 * print(d.as_millis());   // 90000
 */

/**
 * @nog_struct Timestamp
 * @module time
 * @description Represents a point in time with broken-down components.
 * @field year int - Year (e.g., 2024)
 * @field month int - Month (1-12)
 * @field day int - Day of month (1-31)
 * @field hour int - Hour (0-23)
 * @field minute int - Minute (0-59)
 * @field second int - Second (0-59)
 * @field millisecond int - Millisecond (0-999)
 * @field weekday int - Day of week (0=Sunday, 6=Saturday)
 * @example
 * now := time.now();
 * print(now.year, now.month, now.day);
 */

/**
 * @nog_fn millis
 * @module time
 * @description Creates a Duration from milliseconds.
 * @param ms int - Number of milliseconds
 * @returns Duration - A new Duration
 * @example
 * d := time.millis(5000);
 * print(d.as_seconds());  // 5
 */

/**
 * @nog_fn seconds
 * @module time
 * @description Creates a Duration from seconds.
 * @param s int - Number of seconds
 * @returns Duration - A new Duration
 * @example
 * d := time.seconds(90);
 * print(d.as_minutes());  // 1
 */

/**
 * @nog_fn minutes
 * @module time
 * @description Creates a Duration from minutes.
 * @param m int - Number of minutes
 * @returns Duration - A new Duration
 * @example
 * d := time.minutes(5);
 * print(d.as_seconds());  // 300
 */

/**
 * @nog_fn hours
 * @module time
 * @description Creates a Duration from hours.
 * @param h int - Number of hours
 * @returns Duration - A new Duration
 * @example
 * d := time.hours(2);
 * print(d.as_minutes());  // 120
 */

/**
 * @nog_fn days
 * @module time
 * @description Creates a Duration from days.
 * @param d int - Number of days
 * @returns Duration - A new Duration
 * @example
 * d := time.days(1);
 * print(d.as_hours());  // 24
 */

/**
 * @nog_fn now
 * @module time
 * @description Returns the current local time.
 * @returns Timestamp - Current local timestamp
 * @example
 * now := time.now();
 * print(now.year, now.month, now.day);
 */

/**
 * @nog_fn now_utc
 * @module time
 * @description Returns the current UTC time.
 * @returns Timestamp - Current UTC timestamp
 * @example
 * utc := time.now_utc();
 * print(utc.hour);
 */

/**
 * @nog_fn since
 * @module time
 * @description Returns the elapsed duration since a timestamp.
 * @param ts Timestamp - The starting timestamp
 * @returns Duration - Elapsed time since ts
 * @example
 * start := time.now();
 * // ... work ...
 * elapsed := time.since(start);
 * print(elapsed.as_millis());
 */

/**
 * @nog_fn parse
 * @module time
 * @description Parses a timestamp from a string.
 * @param str str - The string to parse
 * @param fmt str - Format string (strftime specifiers)
 * @returns Timestamp or err - Parsed timestamp or error
 * @example
 * ts := time.parse("2024-01-15", "%Y-%m-%d") or fail err;
 * print(ts.year, ts.month, ts.day);
 */

/**
 * @nog_method as_millis
 * @type Duration
 * @description Returns the duration in milliseconds.
 * @returns int - Duration in milliseconds
 * @example
 * d := time.seconds(5);
 * print(d.as_millis());  // 5000
 */

/**
 * @nog_method as_seconds
 * @type Duration
 * @description Returns the duration in seconds.
 * @returns int - Duration in seconds
 * @example
 * d := time.minutes(2);
 * print(d.as_seconds());  // 120
 */

/**
 * @nog_method as_minutes
 * @type Duration
 * @description Returns the duration in minutes.
 * @returns int - Duration in minutes
 * @example
 * d := time.hours(1);
 * print(d.as_minutes());  // 60
 */

/**
 * @nog_method as_hours
 * @type Duration
 * @description Returns the duration in hours.
 * @returns int - Duration in hours
 * @example
 * d := time.days(1);
 * print(d.as_hours());  // 24
 */

/**
 * @nog_method as_days
 * @type Duration
 * @description Returns the duration in days.
 * @returns int - Duration in days
 * @example
 * d := time.hours(48);
 * print(d.as_days());  // 2
 */

/**
 * @nog_method unix
 * @type Timestamp
 * @description Returns Unix timestamp in seconds.
 * @returns int - Unix timestamp in seconds
 * @example
 * now := time.now();
 * print(now.unix());
 */

/**
 * @nog_method unix_millis
 * @type Timestamp
 * @description Returns Unix timestamp in milliseconds.
 * @returns int - Unix timestamp in milliseconds
 * @example
 * now := time.now();
 * print(now.unix_millis());
 */

/**
 * @nog_method format
 * @type Timestamp
 * @description Formats the timestamp using strftime specifiers.
 * @param fmt str - Format string
 * @returns str - Formatted string
 * @example
 * now := time.now();
 * print(now.format("%Y-%m-%d %H:%M:%S"));
 */

#include "time.hpp"

using namespace std;

namespace bishop::stdlib {

/**
 * Creates the AST for the built-in time module.
 */
unique_ptr<Program> create_time_module() {
    auto program = make_unique<Program>();

    // Duration :: struct { millis_value int }
    auto duration_struct = make_unique<StructDef>();
    duration_struct->name = "Duration";
    duration_struct->visibility = Visibility::Public;
    duration_struct->fields.push_back({"millis_value", "int", ""});
    program->structs.push_back(move(duration_struct));

    // Duration methods
    // as_millis(self) -> int
    auto as_millis_method = make_unique<MethodDef>();
    as_millis_method->struct_name = "Duration";
    as_millis_method->name = "as_millis";
    as_millis_method->visibility = Visibility::Public;
    as_millis_method->params.push_back({"Duration", "self"});
    as_millis_method->return_type = "int";
    program->methods.push_back(move(as_millis_method));

    // as_seconds(self) -> int
    auto as_seconds_method = make_unique<MethodDef>();
    as_seconds_method->struct_name = "Duration";
    as_seconds_method->name = "as_seconds";
    as_seconds_method->visibility = Visibility::Public;
    as_seconds_method->params.push_back({"Duration", "self"});
    as_seconds_method->return_type = "int";
    program->methods.push_back(move(as_seconds_method));

    // as_minutes(self) -> int
    auto as_minutes_method = make_unique<MethodDef>();
    as_minutes_method->struct_name = "Duration";
    as_minutes_method->name = "as_minutes";
    as_minutes_method->visibility = Visibility::Public;
    as_minutes_method->params.push_back({"Duration", "self"});
    as_minutes_method->return_type = "int";
    program->methods.push_back(move(as_minutes_method));

    // as_hours(self) -> int
    auto as_hours_method = make_unique<MethodDef>();
    as_hours_method->struct_name = "Duration";
    as_hours_method->name = "as_hours";
    as_hours_method->visibility = Visibility::Public;
    as_hours_method->params.push_back({"Duration", "self"});
    as_hours_method->return_type = "int";
    program->methods.push_back(move(as_hours_method));

    // as_days(self) -> int
    auto as_days_method = make_unique<MethodDef>();
    as_days_method->struct_name = "Duration";
    as_days_method->name = "as_days";
    as_days_method->visibility = Visibility::Public;
    as_days_method->params.push_back({"Duration", "self"});
    as_days_method->return_type = "int";
    program->methods.push_back(move(as_days_method));

    // Timestamp :: struct { year, month, day, hour, minute, second, millisecond, weekday int }
    auto timestamp_struct = make_unique<StructDef>();
    timestamp_struct->name = "Timestamp";
    timestamp_struct->visibility = Visibility::Public;
    timestamp_struct->fields.push_back({"year", "int", ""});
    timestamp_struct->fields.push_back({"month", "int", ""});
    timestamp_struct->fields.push_back({"day", "int", ""});
    timestamp_struct->fields.push_back({"hour", "int", ""});
    timestamp_struct->fields.push_back({"minute", "int", ""});
    timestamp_struct->fields.push_back({"second", "int", ""});
    timestamp_struct->fields.push_back({"millisecond", "int", ""});
    timestamp_struct->fields.push_back({"weekday", "int", ""});
    program->structs.push_back(move(timestamp_struct));

    // Timestamp methods
    // unix(self) -> int
    auto unix_method = make_unique<MethodDef>();
    unix_method->struct_name = "Timestamp";
    unix_method->name = "unix";
    unix_method->visibility = Visibility::Public;
    unix_method->params.push_back({"Timestamp", "self"});
    unix_method->return_type = "int";
    program->methods.push_back(move(unix_method));

    // unix_millis(self) -> int
    auto unix_millis_method = make_unique<MethodDef>();
    unix_millis_method->struct_name = "Timestamp";
    unix_millis_method->name = "unix_millis";
    unix_millis_method->visibility = Visibility::Public;
    unix_millis_method->params.push_back({"Timestamp", "self"});
    unix_millis_method->return_type = "int";
    program->methods.push_back(move(unix_millis_method));

    // format(self, str) -> str
    auto format_method = make_unique<MethodDef>();
    format_method->struct_name = "Timestamp";
    format_method->name = "format";
    format_method->visibility = Visibility::Public;
    format_method->params.push_back({"Timestamp", "self"});
    format_method->params.push_back({"str", "fmt"});
    format_method->return_type = "str";
    program->methods.push_back(move(format_method));

    // Module-level functions

    // fn millis(int ms) -> time.Duration
    auto millis_fn = make_unique<FunctionDef>();
    millis_fn->name = "millis";
    millis_fn->visibility = Visibility::Public;
    millis_fn->params.push_back({"int", "ms"});
    millis_fn->return_type = "time.Duration";
    program->functions.push_back(move(millis_fn));

    // fn seconds(int s) -> time.Duration
    auto seconds_fn = make_unique<FunctionDef>();
    seconds_fn->name = "seconds";
    seconds_fn->visibility = Visibility::Public;
    seconds_fn->params.push_back({"int", "s"});
    seconds_fn->return_type = "time.Duration";
    program->functions.push_back(move(seconds_fn));

    // fn minutes(int m) -> time.Duration
    auto minutes_fn = make_unique<FunctionDef>();
    minutes_fn->name = "minutes";
    minutes_fn->visibility = Visibility::Public;
    minutes_fn->params.push_back({"int", "m"});
    minutes_fn->return_type = "time.Duration";
    program->functions.push_back(move(minutes_fn));

    // fn hours(int h) -> time.Duration
    auto hours_fn = make_unique<FunctionDef>();
    hours_fn->name = "hours";
    hours_fn->visibility = Visibility::Public;
    hours_fn->params.push_back({"int", "h"});
    hours_fn->return_type = "time.Duration";
    program->functions.push_back(move(hours_fn));

    // fn days(int d) -> time.Duration
    auto days_fn = make_unique<FunctionDef>();
    days_fn->name = "days";
    days_fn->visibility = Visibility::Public;
    days_fn->params.push_back({"int", "d"});
    days_fn->return_type = "time.Duration";
    program->functions.push_back(move(days_fn));

    // fn now() -> time.Timestamp
    auto now_fn = make_unique<FunctionDef>();
    now_fn->name = "now";
    now_fn->visibility = Visibility::Public;
    now_fn->return_type = "time.Timestamp";
    program->functions.push_back(move(now_fn));

    // fn now_utc() -> time.Timestamp
    auto now_utc_fn = make_unique<FunctionDef>();
    now_utc_fn->name = "now_utc";
    now_utc_fn->visibility = Visibility::Public;
    now_utc_fn->return_type = "time.Timestamp";
    program->functions.push_back(move(now_utc_fn));

    // fn since(time.Timestamp ts) -> time.Duration
    auto since_fn = make_unique<FunctionDef>();
    since_fn->name = "since";
    since_fn->visibility = Visibility::Public;
    since_fn->params.push_back({"time.Timestamp", "ts"});
    since_fn->return_type = "time.Duration";
    program->functions.push_back(move(since_fn));

    // fn parse(str s, str fmt) -> time.Timestamp or err
    auto parse_fn = make_unique<FunctionDef>();
    parse_fn->name = "parse";
    parse_fn->visibility = Visibility::Public;
    parse_fn->params.push_back({"str", "s"});
    parse_fn->params.push_back({"str", "fmt"});
    parse_fn->return_type = "time.Timestamp";
    parse_fn->error_type = "err";
    program->functions.push_back(move(parse_fn));

    return program;
}

/**
 * Returns empty - time.hpp is included at the top of generated code
 * for precompiled header support.
 */
string generate_time_runtime() {
    return "";
}

}  // namespace bishop::stdlib
