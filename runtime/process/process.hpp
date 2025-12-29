/**
 * @file process.hpp
 * @brief Bishop process runtime library.
 *
 * Provides process execution and environment operations for Bishop programs.
 * This header is included when programs import the process module.
 */

#pragma once

#include <bishop/std.hpp>
#include <bishop/error.hpp>
#include <cstdlib>
#include <cstring>
#include <array>
#include <memory>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>

namespace process {

/**
 * Result of a process execution.
 */
struct ProcessResult {
    std::string output;
    std::string error;
    int exit_code;
    bool success;
};

/**
 * Options for process execution.
 */
struct ProcessOptions {
    std::string cwd;
    std::map<std::string, std::string> env_vars;
    int timeout;
    std::string stdin_str;
    bool capture_stdout;
    bool capture_stderr;
};

/**
 * Executes a command with arguments.
 * Returns a ProcessResult with stdout, stderr, exit_code, and success.
 */
inline bishop::rt::Result<ProcessResult> run(
    const std::string& cmd,
    const std::vector<std::string>& args
) {
    int stdout_pipe[2];
    int stderr_pipe[2];

    if (pipe(stdout_pipe) == -1 || pipe(stderr_pipe) == -1) {
        return std::make_shared<bishop::rt::Error>("Failed to create pipes");
    }

    pid_t pid = fork();

    if (pid == -1) {
        close(stdout_pipe[0]);
        close(stdout_pipe[1]);
        close(stderr_pipe[0]);
        close(stderr_pipe[1]);
        return std::make_shared<bishop::rt::Error>("Failed to fork process");
    }

    if (pid == 0) {
        // Child process
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stderr_pipe[1], STDERR_FILENO);

        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        // Build argv array
        std::vector<char*> argv;
        argv.push_back(const_cast<char*>(cmd.c_str()));

        for (const auto& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }

        argv.push_back(nullptr);

        execvp(cmd.c_str(), argv.data());
        _exit(127);
    }

    // Parent process
    close(stdout_pipe[1]);
    close(stderr_pipe[1]);

    // Read stdout
    std::string stdout_output;
    char buffer[4096];
    ssize_t bytes_read;

    while ((bytes_read = read(stdout_pipe[0], buffer, sizeof(buffer))) > 0) {
        stdout_output.append(buffer, bytes_read);
    }

    close(stdout_pipe[0]);

    // Read stderr
    std::string stderr_output;

    while ((bytes_read = read(stderr_pipe[0], buffer, sizeof(buffer))) > 0) {
        stderr_output.append(buffer, bytes_read);
    }

    close(stderr_pipe[0]);

    // Wait for child
    int status;
    waitpid(pid, &status, 0);

    ProcessResult result;
    result.output = stdout_output;
    result.error = stderr_output;

    if (WIFEXITED(status)) {
        result.exit_code = WEXITSTATUS(status);
    } else {
        result.exit_code = -1;
    }

    result.success = (result.exit_code == 0);

    return result;
}

/**
 * Executes a command with arguments and options.
 * Returns a ProcessResult with stdout, stderr, exit_code, and success.
 */
inline bishop::rt::Result<ProcessResult> run_with_options(
    const std::string& cmd,
    const std::vector<std::string>& args,
    const ProcessOptions& options
) {
    int stdout_pipe[2];
    int stderr_pipe[2];
    int stdin_pipe[2];

    if (pipe(stdout_pipe) == -1 || pipe(stderr_pipe) == -1) {
        return std::make_shared<bishop::rt::Error>("Failed to create pipes");
    }

    bool has_stdin = !options.stdin_str.empty();

    if (has_stdin && pipe(stdin_pipe) == -1) {
        close(stdout_pipe[0]);
        close(stdout_pipe[1]);
        close(stderr_pipe[0]);
        close(stderr_pipe[1]);
        return std::make_shared<bishop::rt::Error>("Failed to create stdin pipe");
    }

    pid_t pid = fork();

    if (pid == -1) {
        close(stdout_pipe[0]);
        close(stdout_pipe[1]);
        close(stderr_pipe[0]);
        close(stderr_pipe[1]);

        if (has_stdin) {
            close(stdin_pipe[0]);
            close(stdin_pipe[1]);
        }

        return std::make_shared<bishop::rt::Error>("Failed to fork process");
    }

    if (pid == 0) {
        // Child process
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

        if (has_stdin) {
            close(stdin_pipe[1]);
            dup2(stdin_pipe[0], STDIN_FILENO);
            close(stdin_pipe[0]);
        }

        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stderr_pipe[1], STDERR_FILENO);

        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        // Change working directory if specified
        if (!options.cwd.empty()) {
            if (chdir(options.cwd.c_str()) == -1) {
                _exit(127);
            }
        }

        // Set environment variables
        for (const auto& [key, value] : options.env_vars) {
            setenv(key.c_str(), value.c_str(), 1);
        }

        // Build argv array
        std::vector<char*> argv;
        argv.push_back(const_cast<char*>(cmd.c_str()));

        for (const auto& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }

        argv.push_back(nullptr);

        execvp(cmd.c_str(), argv.data());
        _exit(127);
    }

    // Parent process
    close(stdout_pipe[1]);
    close(stderr_pipe[1]);

    if (has_stdin) {
        close(stdin_pipe[0]);
        write(stdin_pipe[1], options.stdin_str.c_str(), options.stdin_str.size());
        close(stdin_pipe[1]);
    }

    // Set up timeout if specified
    bool timed_out = false;

    if (options.timeout > 0) {
        // Set non-blocking mode on pipes
        fcntl(stdout_pipe[0], F_SETFL, O_NONBLOCK);
        fcntl(stderr_pipe[0], F_SETFL, O_NONBLOCK);

        auto start_time = std::chrono::steady_clock::now();
        auto timeout_ms = std::chrono::milliseconds(options.timeout);

        // Poll for completion with timeout
        while (true) {
            int status;
            pid_t result = waitpid(pid, &status, WNOHANG);

            if (result == pid) {
                break;
            }

            auto elapsed = std::chrono::steady_clock::now() - start_time;

            if (elapsed >= timeout_ms) {
                kill(pid, SIGKILL);
                waitpid(pid, &status, 0);
                timed_out = true;
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    // Read stdout
    std::string stdout_output;
    char buffer[4096];
    ssize_t bytes_read;

    // Reset to blocking mode for final read
    fcntl(stdout_pipe[0], F_SETFL, 0);

    while ((bytes_read = read(stdout_pipe[0], buffer, sizeof(buffer))) > 0) {
        stdout_output.append(buffer, bytes_read);
    }

    close(stdout_pipe[0]);

    // Read stderr
    std::string stderr_output;
    fcntl(stderr_pipe[0], F_SETFL, 0);

    while ((bytes_read = read(stderr_pipe[0], buffer, sizeof(buffer))) > 0) {
        stderr_output.append(buffer, bytes_read);
    }

    close(stderr_pipe[0]);

    // Wait for child if we haven't already
    int status;

    if (options.timeout <= 0) {
        waitpid(pid, &status, 0);
    }

    if (timed_out) {
        return std::make_shared<bishop::rt::Error>("Process timed out");
    }

    ProcessResult proc_result;
    proc_result.output = stdout_output;
    proc_result.error = stderr_output;

    if (WIFEXITED(status)) {
        proc_result.exit_code = WEXITSTATUS(status);
    } else {
        proc_result.exit_code = -1;
    }

    proc_result.success = (proc_result.exit_code == 0);

    return proc_result;
}

/**
 * Executes a shell command.
 * Supports pipes and redirects.
 */
inline bishop::rt::Result<ProcessResult> shell(const std::string& cmd) {
    std::vector<std::string> args = {"-c", cmd};
    return run("/bin/sh", args);
}

/**
 * Gets an environment variable value.
 * Returns error if variable is not found.
 */
inline bishop::rt::Result<std::string> env(const std::string& name) {
    const char* value = std::getenv(name.c_str());

    if (value == nullptr) {
        return std::make_shared<bishop::rt::Error>(
            "Environment variable not found: " + name);
    }

    return std::string(value);
}

/**
 * Sets an environment variable.
 * Returns error if the operation fails.
 */
inline bishop::rt::Result<bool> set_env(const std::string& name, const std::string& value) {
    if (name.empty()) {
        return std::make_shared<bishop::rt::Error>(
            "Environment variable name cannot be empty");
    }

    if (setenv(name.c_str(), value.c_str(), 1) != 0) {
        return std::make_shared<bishop::rt::Error>(
            "Failed to set environment variable: " + name + " - " + strerror(errno));
    }

    return true;
}

/**
 * Gets the current working directory.
 * Returns error if the operation fails.
 */
inline bishop::rt::Result<std::string> cwd() {
    char buffer[4096];

    if (getcwd(buffer, sizeof(buffer)) == nullptr) {
        return std::make_shared<bishop::rt::Error>(
            "Failed to get current working directory: " + std::string(strerror(errno)));
    }

    return std::string(buffer);
}

/**
 * Changes the current working directory.
 * Returns true on success, false on failure.
 */
inline bishop::rt::Result<bool> chdir(const std::string& path) {
    if (::chdir(path.c_str()) == 0) {
        return true;
    }

    return std::make_shared<bishop::rt::Error>("Failed to change directory to: " + path);
}

/**
 * Gets command line arguments.
 * Note: This is set by the runtime at program start.
 */
inline std::vector<std::string>& args_storage() {
    static std::vector<std::string> args;
    return args;
}

/**
 * Initializes the args from main().
 */
inline void init_args(int argc, char* argv[]) {
    args_storage().clear();

    for (int i = 0; i < argc; i++) {
        args_storage().push_back(argv[i]);
    }
}

/**
 * Gets command line arguments as a List<str>.
 */
inline std::vector<std::string> args() {
    return args_storage();
}

}  // namespace process
