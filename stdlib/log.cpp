/**
 * @file log.cpp
 * @brief Built-in log (logging) module implementation.
 *
 * Creates the AST definitions for the log module.
 * The actual runtime is in src/runtime/log/log.hpp and included as a header.
 */

/**
 * @nog_fn debug
 * @module log
 * @description Logs a debug-level message.
 * @param message str - The message to log
 * @returns void
 * @example
 * import log;
 * log.debug("Entering function foo");
 */

/**
 * @nog_fn debug_kv
 * @module log
 * @description Logs a debug-level message with a key-value pair.
 * @param message str - The message to log
 * @param key str - The key for additional context
 * @param value str - The value for additional context
 * @returns void
 * @example
 * import log;
 * log.debug_kv("Processing item", "item_id", "123");
 */

/**
 * @nog_fn info
 * @module log
 * @description Logs an info-level message.
 * @param message str - The message to log
 * @returns void
 * @example
 * import log;
 * log.info("Server started");
 */

/**
 * @nog_fn info_kv
 * @module log
 * @description Logs an info-level message with a key-value pair.
 * @param message str - The message to log
 * @param key str - The key for additional context
 * @param value str - The value for additional context
 * @returns void
 * @example
 * import log;
 * log.info_kv("User logged in", "user_id", "12345");
 */

/**
 * @nog_fn warn
 * @module log
 * @description Logs a warning-level message.
 * @param message str - The message to log
 * @returns void
 * @example
 * import log;
 * log.warn("Disk space low");
 */

/**
 * @nog_fn warn_kv
 * @module log
 * @description Logs a warning-level message with a key-value pair.
 * @param message str - The message to log
 * @param key str - The key for additional context
 * @param value str - The value for additional context
 * @returns void
 * @example
 * import log;
 * log.warn_kv("High memory usage", "percent", "95");
 */

/**
 * @nog_fn error
 * @module log
 * @description Logs an error-level message.
 * @param message str - The message to log
 * @returns void
 * @example
 * import log;
 * log.error("Failed to connect to database");
 */

/**
 * @nog_fn error_kv
 * @module log
 * @description Logs an error-level message with a key-value pair.
 * @param message str - The message to log
 * @param key str - The key for additional context
 * @param value str - The value for additional context
 * @returns void
 * @example
 * import log;
 * log.error_kv("Connection failed", "host", "example.com");
 */

/**
 * @nog_fn set_level
 * @module log
 * @description Sets the minimum logging level. Messages below this level are suppressed.
 * @param level int - The log level (log.DEBUG, log.INFO, log.WARN, log.ERROR)
 * @returns void
 * @example
 * import log;
 * log.set_level(log.INFO);  // Suppress debug messages
 */

/**
 * @nog_fn set_format
 * @module log
 * @description Sets the log output format using strftime-style specifiers.
 * @param format str - Format string (e.g., "[%Y-%m-%d %H:%M:%S] [%l] %v")
 * @returns void
 * @example
 * import log;
 * log.set_format("[%H:%M:%S] %v");
 */

/**
 * @nog_fn add_file
 * @module log
 * @description Adds a file as a logging output destination.
 * @param path str - Path to the log file
 * @returns void
 * @example
 * import log;
 * log.add_file("app.log");
 */

/**
 * @nog_fn add_file_level
 * @module log
 * @description Adds a file as a logging output with a specific minimum level.
 * @param path str - Path to the log file
 * @param level int - Minimum log level for this file
 * @returns void
 * @example
 * import log;
 * log.add_file_level("errors.log", log.ERROR);
 */

#include "log.hpp"

using namespace std;

namespace bishop::stdlib {

/**
 * Creates the AST for the built-in log module.
 */
unique_ptr<Program> create_log_module() {
    auto program = make_unique<Program>();

    // Constants for log levels
    // const int DEBUG = 0
    auto debug_const = make_unique<VariableDecl>();
    debug_const->name = "DEBUG";
    debug_const->type = "int";
    debug_const->is_const = true;
    program->constants.push_back(move(debug_const));

    // const int INFO = 1
    auto info_const = make_unique<VariableDecl>();
    info_const->name = "INFO";
    info_const->type = "int";
    info_const->is_const = true;
    program->constants.push_back(move(info_const));

    // const int WARN = 2
    auto warn_const = make_unique<VariableDecl>();
    warn_const->name = "WARN";
    warn_const->type = "int";
    warn_const->is_const = true;
    program->constants.push_back(move(warn_const));

    // const int ERROR = 3
    auto error_const = make_unique<VariableDecl>();
    error_const->name = "ERROR";
    error_const->type = "int";
    error_const->is_const = true;
    program->constants.push_back(move(error_const));

    // fn debug(str message) -> void
    auto debug_fn = make_unique<FunctionDef>();
    debug_fn->name = "debug";
    debug_fn->visibility = Visibility::Public;
    debug_fn->params.push_back({"str", "message"});
    debug_fn->return_type = "void";
    program->functions.push_back(move(debug_fn));

    // fn debug_kv(str message, str key, str value) -> void
    auto debug_kv_fn = make_unique<FunctionDef>();
    debug_kv_fn->name = "debug_kv";
    debug_kv_fn->visibility = Visibility::Public;
    debug_kv_fn->params.push_back({"str", "message"});
    debug_kv_fn->params.push_back({"str", "key"});
    debug_kv_fn->params.push_back({"str", "value"});
    debug_kv_fn->return_type = "void";
    program->functions.push_back(move(debug_kv_fn));

    // fn info(str message) -> void
    auto info_fn = make_unique<FunctionDef>();
    info_fn->name = "info";
    info_fn->visibility = Visibility::Public;
    info_fn->params.push_back({"str", "message"});
    info_fn->return_type = "void";
    program->functions.push_back(move(info_fn));

    // fn info_kv(str message, str key, str value) -> void
    auto info_kv_fn = make_unique<FunctionDef>();
    info_kv_fn->name = "info_kv";
    info_kv_fn->visibility = Visibility::Public;
    info_kv_fn->params.push_back({"str", "message"});
    info_kv_fn->params.push_back({"str", "key"});
    info_kv_fn->params.push_back({"str", "value"});
    info_kv_fn->return_type = "void";
    program->functions.push_back(move(info_kv_fn));

    // fn warn(str message) -> void
    auto warn_fn = make_unique<FunctionDef>();
    warn_fn->name = "warn";
    warn_fn->visibility = Visibility::Public;
    warn_fn->params.push_back({"str", "message"});
    warn_fn->return_type = "void";
    program->functions.push_back(move(warn_fn));

    // fn warn_kv(str message, str key, str value) -> void
    auto warn_kv_fn = make_unique<FunctionDef>();
    warn_kv_fn->name = "warn_kv";
    warn_kv_fn->visibility = Visibility::Public;
    warn_kv_fn->params.push_back({"str", "message"});
    warn_kv_fn->params.push_back({"str", "key"});
    warn_kv_fn->params.push_back({"str", "value"});
    warn_kv_fn->return_type = "void";
    program->functions.push_back(move(warn_kv_fn));

    // fn error(str message) -> void
    auto error_fn = make_unique<FunctionDef>();
    error_fn->name = "error";
    error_fn->visibility = Visibility::Public;
    error_fn->params.push_back({"str", "message"});
    error_fn->return_type = "void";
    program->functions.push_back(move(error_fn));

    // fn error_kv(str message, str key, str value) -> void
    auto error_kv_fn = make_unique<FunctionDef>();
    error_kv_fn->name = "error_kv";
    error_kv_fn->visibility = Visibility::Public;
    error_kv_fn->params.push_back({"str", "message"});
    error_kv_fn->params.push_back({"str", "key"});
    error_kv_fn->params.push_back({"str", "value"});
    error_kv_fn->return_type = "void";
    program->functions.push_back(move(error_kv_fn));

    // fn set_level(int level) -> void
    auto set_level_fn = make_unique<FunctionDef>();
    set_level_fn->name = "set_level";
    set_level_fn->visibility = Visibility::Public;
    set_level_fn->params.push_back({"int", "level"});
    set_level_fn->return_type = "void";
    program->functions.push_back(move(set_level_fn));

    // fn set_format(str format) -> void
    auto set_format_fn = make_unique<FunctionDef>();
    set_format_fn->name = "set_format";
    set_format_fn->visibility = Visibility::Public;
    set_format_fn->params.push_back({"str", "format"});
    set_format_fn->return_type = "void";
    program->functions.push_back(move(set_format_fn));

    // fn add_file(str path) -> void
    auto add_file_fn = make_unique<FunctionDef>();
    add_file_fn->name = "add_file";
    add_file_fn->visibility = Visibility::Public;
    add_file_fn->params.push_back({"str", "path"});
    add_file_fn->return_type = "void";
    program->functions.push_back(move(add_file_fn));

    // fn add_file_level(str path, int level) -> void
    auto add_file_level_fn = make_unique<FunctionDef>();
    add_file_level_fn->name = "add_file_level";
    add_file_level_fn->visibility = Visibility::Public;
    add_file_level_fn->params.push_back({"str", "path"});
    add_file_level_fn->params.push_back({"int", "level"});
    add_file_level_fn->return_type = "void";
    program->functions.push_back(move(add_file_level_fn));

    return program;
}

/**
 * Returns empty - log.hpp is included at the top of generated code
 * for precompiled header support.
 */
string generate_log_runtime() {
    return "";
}

}  // namespace bishop::stdlib
