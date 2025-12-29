/**
 * @file process.cpp
 * @brief Built-in process module implementation.
 *
 * Creates the AST definitions for the process module.
 * The actual runtime is in runtime/process/process.hpp and included as a header.
 */

/**
 * @nog_struct ProcessResult
 * @module process
 * @description Result of a process execution.
 * @field output str - Standard output from the process
 * @field error str - Standard error from the process
 * @field exit_code int - Exit code of the process
 * @field success bool - True if exit code is 0
 * @example
 * result := process.run("ls", ["-la"]) or return;
 * print(result.output);
 * print("Exit code:", result.exit_code);
 */

/**
 * @nog_struct ProcessOptions
 * @module process
 * @description Options for process execution.
 * @field cwd str - Working directory for the process
 * @field env Map<str, str> - Environment variables to set
 * @field timeout int - Timeout in milliseconds (0 = no timeout)
 * @field stdin str - Input to send to the process
 * @field capture_stdout bool - Whether to capture stdout
 * @field capture_stderr bool - Whether to capture stderr
 * @example
 * result := process.run("npm", ["install"], ProcessOptions {
 *     cwd: "/path/to/project",
 *     timeout: 30000
 * });
 */

/**
 * @nog_fn run
 * @module process
 * @description Executes a command with arguments.
 * @param cmd str - Command to execute
 * @param args List<str> - Arguments to pass to the command
 * @returns ProcessResult or err - Result of the execution
 * @example
 * result := process.run("ls", ["-la"]) or {
 *     print("Command failed:", err.message);
 *     return;
 * };
 * print(result.output);
 */

/**
 * @nog_fn shell
 * @module process
 * @description Executes a shell command with pipe and redirect support.
 * @param cmd str - Shell command to execute
 * @returns ProcessResult or err - Result of the execution
 * @example
 * result := process.shell("ls -la | grep txt") or return;
 * print(result.output);
 */

/**
 * @nog_fn env
 * @module process
 * @description Gets an environment variable value.
 * @param name str - Name of the environment variable
 * @returns str - Value of the variable, or empty string if not found
 * @example
 * home := process.env("HOME") default "/tmp";
 */

/**
 * @nog_fn set_env
 * @module process
 * @description Sets an environment variable.
 * @param name str - Name of the environment variable
 * @param value str - Value to set
 * @example
 * process.set_env("MY_VAR", "value");
 */

/**
 * @nog_fn cwd
 * @module process
 * @description Gets the current working directory.
 * @returns str - Current working directory path
 * @example
 * print(process.cwd());
 */

/**
 * @nog_fn chdir
 * @module process
 * @description Changes the current working directory.
 * @param path str - Path to change to
 * @returns bool or err - True on success, error on failure
 * @example
 * process.chdir("/new/dir") or {
 *     print("Failed to change directory");
 *     return;
 * };
 */

/**
 * @nog_fn args
 * @module process
 * @description Gets command line arguments.
 * @returns List<str> - List of command line arguments
 * @example
 * args := process.args();
 * for arg in args {
 *     print(arg);
 * }
 */

#include "process.hpp"

using namespace std;

namespace nog::stdlib {

/**
 * Creates the AST for the built-in process module.
 */
unique_ptr<Program> create_process_module() {
    auto program = make_unique<Program>();

    // ProcessResult :: struct { output str, error str, exit_code int, success bool }
    auto result_struct = make_unique<StructDef>();
    result_struct->name = "ProcessResult";
    result_struct->visibility = Visibility::Public;
    result_struct->fields.push_back({"output", "str", ""});
    result_struct->fields.push_back({"error", "str", ""});
    result_struct->fields.push_back({"exit_code", "int", ""});
    result_struct->fields.push_back({"success", "bool", ""});
    program->structs.push_back(move(result_struct));

    // ProcessOptions :: struct { cwd str, timeout int, stdin str }
    auto options_struct = make_unique<StructDef>();
    options_struct->name = "ProcessOptions";
    options_struct->visibility = Visibility::Public;
    options_struct->fields.push_back({"cwd", "str", ""});
    options_struct->fields.push_back({"timeout", "int", ""});
    options_struct->fields.push_back({"stdin", "str", ""});
    program->structs.push_back(move(options_struct));

    // fn run(str cmd, List<str> args) -> process.ProcessResult or err
    auto run_fn = make_unique<FunctionDef>();
    run_fn->name = "run";
    run_fn->visibility = Visibility::Public;
    run_fn->params.push_back({"str", "cmd"});
    run_fn->params.push_back({"List<str>", "args"});
    run_fn->return_type = "process.ProcessResult";
    run_fn->error_type = "err";
    program->functions.push_back(move(run_fn));

    // fn shell(str cmd) -> process.ProcessResult or err
    auto shell_fn = make_unique<FunctionDef>();
    shell_fn->name = "shell";
    shell_fn->visibility = Visibility::Public;
    shell_fn->params.push_back({"str", "cmd"});
    shell_fn->return_type = "process.ProcessResult";
    shell_fn->error_type = "err";
    program->functions.push_back(move(shell_fn));

    // fn env(str name) -> str
    auto env_fn = make_unique<FunctionDef>();
    env_fn->name = "env";
    env_fn->visibility = Visibility::Public;
    env_fn->params.push_back({"str", "name"});
    env_fn->return_type = "str";
    program->functions.push_back(move(env_fn));

    // fn set_env(str name, str value)
    auto set_env_fn = make_unique<FunctionDef>();
    set_env_fn->name = "set_env";
    set_env_fn->visibility = Visibility::Public;
    set_env_fn->params.push_back({"str", "name"});
    set_env_fn->params.push_back({"str", "value"});
    program->functions.push_back(move(set_env_fn));

    // fn cwd() -> str
    auto cwd_fn = make_unique<FunctionDef>();
    cwd_fn->name = "cwd";
    cwd_fn->visibility = Visibility::Public;
    cwd_fn->return_type = "str";
    program->functions.push_back(move(cwd_fn));

    // fn chdir(str path) -> bool or err
    auto chdir_fn = make_unique<FunctionDef>();
    chdir_fn->name = "chdir";
    chdir_fn->visibility = Visibility::Public;
    chdir_fn->params.push_back({"str", "path"});
    chdir_fn->return_type = "bool";
    chdir_fn->error_type = "err";
    program->functions.push_back(move(chdir_fn));

    // fn args() -> List<str>
    auto args_fn = make_unique<FunctionDef>();
    args_fn->name = "args";
    args_fn->visibility = Visibility::Public;
    args_fn->return_type = "List<str>";
    program->functions.push_back(move(args_fn));

    return program;
}

/**
 * Returns empty - process.hpp is included at the top of generated code
 * for precompiled header support.
 */
string generate_process_runtime() {
    return "";
}

}  // namespace nog::stdlib
