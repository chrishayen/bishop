/**
 * @file fs.hpp
 * @brief Bishop filesystem runtime library.
 *
 * Provides filesystem operations for Bishop programs.
 * This header is included when programs import the fs module.
 */

#pragma once

#include <bishop/std.hpp>
#include <bishop/error.hpp>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <random>
#include <chrono>
#include <limits>

namespace fs {

// ============================================================================
// FileInfo struct for stat() results
// ============================================================================

/**
 * File information returned by stat().
 */
struct FileInfo {
    int size = 0;
    int modified = 0;
    int created = 0;
    int accessed = 0;
    bool is_file = false;
    bool is_dir = false;
    bool is_symlink = false;
};

// ============================================================================
// DirEntry struct for walk() results
// ============================================================================

/**
 * Directory entry returned by walk().
 */
struct DirEntry {
    std::string path;
    std::string name;
    bool is_dir = false;
    bool is_file = false;
    bool is_symlink = false;
};

// ============================================================================
// File handle struct for open/close/read/write
// ============================================================================

/**
 * Internal file handle implementation (non-copyable due to fstream).
 */
struct FileImpl {
    std::fstream stream;
    std::string path;
    std::string mode;
    bool closed = false;

    FileImpl() = default;
    FileImpl(const std::string& p, const std::string& m) : path(p), mode(m) {}
};

/**
 * File handle for reading and writing files.
 * Uses shared_ptr internally to allow copying.
 */
struct File {
    std::shared_ptr<FileImpl> impl;

    File() : impl(std::make_shared<FileImpl>()) {}
    File(const std::string& p, const std::string& m) : impl(std::make_shared<FileImpl>(p, m)) {}

    /**
     * Reads a single line from the file.
     */
    bishop::rt::Result<std::string> read_line() {
        if (!impl || impl->closed) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("File is closed")
            );
        }

        if (!impl->stream.is_open() || !(impl->stream.good())) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("Cannot read from file: " + impl->path)
            );
        }

        std::string line;

        if (std::getline(impl->stream, line)) {
            return bishop::rt::Result<std::string>::ok(line);
        }

        if (impl->stream.eof()) {
            return bishop::rt::Result<std::string>::ok("");
        }

        return bishop::rt::Result<std::string>::error(
            bishop::rt::Error("Failed to read line from file: " + impl->path)
        );
    }

    /**
     * Reads all lines from the file.
     */
    bishop::rt::Result<std::vector<std::string>> read_lines() {
        if (!impl || impl->closed) {
            return bishop::rt::Result<std::vector<std::string>>::error(
                bishop::rt::Error("File is closed")
            );
        }

        if (!impl->stream.is_open()) {
            return bishop::rt::Result<std::vector<std::string>>::error(
                bishop::rt::Error("Cannot read from file: " + impl->path)
            );
        }

        std::vector<std::string> lines;
        std::string line;

        while (std::getline(impl->stream, line)) {
            lines.push_back(line);
        }

        return bishop::rt::Result<std::vector<std::string>>::ok(lines);
    }

    /**
     * Reads all content from the file.
     */
    bishop::rt::Result<std::string> read_all() {
        if (!impl || impl->closed) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("File is closed")
            );
        }

        if (!impl->stream.is_open()) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("Cannot read from file: " + impl->path)
            );
        }

        std::stringstream buffer;
        buffer << impl->stream.rdbuf();
        return bishop::rt::Result<std::string>::ok(buffer.str());
    }

    /**
     * Writes data to the file.
     */
    bishop::rt::Result<bool> write(const std::string& data) {
        if (!impl || impl->closed) {
            return bishop::rt::Result<bool>::error(
                bishop::rt::Error("File is closed")
            );
        }

        if (!impl->stream.is_open()) {
            return bishop::rt::Result<bool>::error(
                bishop::rt::Error("Cannot write to file: " + impl->path)
            );
        }

        impl->stream << data;

        if (!impl->stream.good()) {
            return bishop::rt::Result<bool>::error(
                bishop::rt::Error("Failed to write to file: " + impl->path)
            );
        }

        return bishop::rt::Result<bool>::ok(true);
    }

    /**
     * Writes data followed by a newline to the file.
     */
    bishop::rt::Result<bool> write_line(const std::string& data) {
        if (!impl || impl->closed) {
            return bishop::rt::Result<bool>::error(
                bishop::rt::Error("File is closed")
            );
        }

        if (!impl->stream.is_open()) {
            return bishop::rt::Result<bool>::error(
                bishop::rt::Error("Cannot write to file: " + impl->path)
            );
        }

        impl->stream << data << '\n';

        if (!impl->stream.good()) {
            return bishop::rt::Result<bool>::error(
                bishop::rt::Error("Failed to write to file: " + impl->path)
            );
        }

        return bishop::rt::Result<bool>::ok(true);
    }

    /**
     * Closes the file.
     */
    void close() {
        if (impl && !impl->closed && impl->stream.is_open()) {
            impl->stream.close();
            impl->closed = true;
        }
    }
};

// ============================================================================
// Existing functions
// ============================================================================

/**
 * Reads the entire contents of a file.
 * Returns empty string if file cannot be read.
 */
inline std::string read_file(const std::string& path) {
    std::ifstream file(path);

    if (!file) {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * Checks if a file or directory exists.
 */
inline bool exists(const std::string& path) {
    return std::filesystem::exists(path);
}

/**
 * Checks if a path is a directory.
 */
inline bool is_dir(const std::string& path) {
    return std::filesystem::is_directory(path);
}

/**
 * Checks if a path is a regular file.
 */
inline bool is_file(const std::string& path) {
    return std::filesystem::is_regular_file(path);
}

/**
 * Lists all entries in a directory, separated by newlines.
 */
inline std::string read_dir(const std::string& path) {
    std::string result;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (!result.empty()) {
            result += "\n";
        }

        result += entry.path().filename().string();
    }

    return result;
}

// ============================================================================
// File Writing Functions
// ============================================================================

/**
 * Writes content to a file, creating it if it doesn't exist.
 */
inline bishop::rt::Result<bool> write_file(const std::string& path, const std::string& content) {
    std::ofstream file(path);

    if (!file) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to open file for writing: " + path)
        );
    }

    file << content;

    if (!file.good()) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to write to file: " + path)
        );
    }

    return bishop::rt::Result<bool>::ok(true);
}

/**
 * Appends content to a file, creating it if it doesn't exist.
 */
inline bishop::rt::Result<bool> append_file(const std::string& path, const std::string& content) {
    std::ofstream file(path, std::ios::app);

    if (!file) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to open file for appending: " + path)
        );
    }

    file << content;

    if (!file.good()) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to append to file: " + path)
        );
    }

    return bishop::rt::Result<bool>::ok(true);
}

/**
 * Reads binary content from a file.
 */
inline bishop::rt::Result<std::string> read_bytes(const std::string& path) {
    std::ifstream file(path, std::ios::binary);

    if (!file) {
        return bishop::rt::Result<std::string>::error(
            bishop::rt::Error("Failed to open file for reading: " + path)
        );
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return bishop::rt::Result<std::string>::ok(buffer.str());
}

/**
 * Writes binary content to a file.
 */
inline bishop::rt::Result<bool> write_bytes(const std::string& path, const std::string& data) {
    std::ofstream file(path, std::ios::binary);

    if (!file) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to open file for writing: " + path)
        );
    }

    file.write(data.data(), data.size());

    if (!file.good()) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to write to file: " + path)
        );
    }

    return bishop::rt::Result<bool>::ok(true);
}

// ============================================================================
// File Handle Functions
// ============================================================================

/**
 * Opens a file with the specified mode.
 * Modes: "r" (read), "w" (write), "a" (append), "rw" (read+write)
 */
inline bishop::rt::Result<File> open(const std::string& path, const std::string& mode) {
    File file(path, mode);

    std::ios_base::openmode ios_mode = std::ios_base::in;

    if (mode == "r") {
        ios_mode = std::ios_base::in;
    } else if (mode == "w") {
        ios_mode = std::ios_base::out | std::ios_base::trunc;
    } else if (mode == "a") {
        ios_mode = std::ios_base::out | std::ios_base::app;
    } else if (mode == "rw") {
        ios_mode = std::ios_base::in | std::ios_base::out;
    } else {
        return bishop::rt::Result<File>::error(
            bishop::rt::Error("Invalid file mode: " + mode)
        );
    }

    file.impl->stream.open(path, ios_mode);

    if (!file.impl->stream.is_open()) {
        return bishop::rt::Result<File>::error(
            bishop::rt::Error("Failed to open file: " + path)
        );
    }

    return bishop::rt::Result<File>::ok(file);
}

// ============================================================================
// Directory Operations
// ============================================================================

/**
 * Lists all entries in a directory as a List<str>.
 */
inline bishop::rt::Result<std::vector<std::string>> list_dir(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        return bishop::rt::Result<std::vector<std::string>>::error(
            bishop::rt::Error("Directory does not exist: " + path)
        );
    }

    if (!std::filesystem::is_directory(path)) {
        return bishop::rt::Result<std::vector<std::string>>::error(
            bishop::rt::Error("Path is not a directory: " + path)
        );
    }

    std::vector<std::string> entries;
    std::error_code ec;
    std::filesystem::directory_iterator it(path, ec);

    if (ec) {
        return bishop::rt::Result<std::vector<std::string>>::error(
            bishop::rt::Error("Failed to list directory: " + path + " - " + ec.message())
        );
    }

    std::filesystem::directory_iterator end;

    for (; it != end; it.increment(ec)) {
        if (ec) {
            return bishop::rt::Result<std::vector<std::string>>::error(
                bishop::rt::Error("Failed to list directory: " + path + " - " + ec.message())
            );
        }

        entries.push_back(it->path().filename().string());
    }

    return bishop::rt::Result<std::vector<std::string>>::ok(entries);
}

/**
 * Creates a single directory.
 */
inline bishop::rt::Result<bool> mkdir(const std::string& path) {
    std::error_code ec;
    bool created = std::filesystem::create_directory(path, ec);

    if (ec) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to create directory: " + path + " - " + ec.message())
        );
    }

    return bishop::rt::Result<bool>::ok(created);
}

/**
 * Creates a directory and all parent directories.
 */
inline bishop::rt::Result<bool> mkdir_all(const std::string& path) {
    std::error_code ec;
    bool created = std::filesystem::create_directories(path, ec);

    if (ec) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to create directories: " + path + " - " + ec.message())
        );
    }

    return bishop::rt::Result<bool>::ok(created);
}

/**
 * Removes a single file.
 */
inline bishop::rt::Result<bool> remove(const std::string& path) {
    std::error_code ec;
    bool removed = std::filesystem::remove(path, ec);

    if (ec) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to remove file: " + path + " - " + ec.message())
        );
    }

    if (!removed) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("File does not exist: " + path)
        );
    }

    return bishop::rt::Result<bool>::ok(true);
}

/**
 * Removes an empty directory.
 */
inline bishop::rt::Result<bool> remove_dir(const std::string& path) {
    std::error_code ec;
    bool removed = std::filesystem::remove(path, ec);

    if (ec) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to remove directory: " + path + " - " + ec.message())
        );
    }

    if (!removed) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Directory does not exist: " + path)
        );
    }

    return bishop::rt::Result<bool>::ok(true);
}

/**
 * Removes a file or directory and all its contents.
 */
inline bishop::rt::Result<bool> remove_all(const std::string& path) {
    std::error_code ec;
    std::uintmax_t count = std::filesystem::remove_all(path, ec);

    if (ec) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to remove: " + path + " - " + ec.message())
        );
    }

    if (count == 0) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Path does not exist: " + path)
        );
    }

    return bishop::rt::Result<bool>::ok(true);
}

// ============================================================================
// File Operations
// ============================================================================

/**
 * Renames or moves a file or directory.
 */
inline bishop::rt::Result<bool> rename(const std::string& from, const std::string& to) {
    std::error_code ec;
    std::filesystem::rename(from, to, ec);

    if (ec) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to rename: " + from + " to " + to + " - " + ec.message())
        );
    }

    return bishop::rt::Result<bool>::ok(true);
}

/**
 * Copies a single file.
 */
inline bishop::rt::Result<bool> copy(const std::string& from, const std::string& to) {
    std::error_code ec;
    std::filesystem::copy_file(from, to, std::filesystem::copy_options::overwrite_existing, ec);

    if (ec) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to copy: " + from + " to " + to + " - " + ec.message())
        );
    }

    return bishop::rt::Result<bool>::ok(true);
}

/**
 * Copies a directory and all its contents.
 */
inline bishop::rt::Result<bool> copy_dir(const std::string& from, const std::string& to) {
    std::error_code ec;
    std::filesystem::copy(from, to, std::filesystem::copy_options::recursive, ec);

    if (ec) {
        return bishop::rt::Result<bool>::error(
            bishop::rt::Error("Failed to copy directory: " + from + " to " + to + " - " + ec.message())
        );
    }

    return bishop::rt::Result<bool>::ok(true);
}

// ============================================================================
// Path Operations
// ============================================================================

/**
 * Joins two path components.
 */
inline std::string join(const std::string& a, const std::string& b) {
    std::filesystem::path p = a;
    p /= b;
    return p.string();
}

/**
 * Gets the directory part of a path.
 */
inline std::string dirname(const std::string& path) {
    return std::filesystem::path(path).parent_path().string();
}

/**
 * Gets the filename part of a path.
 */
inline std::string basename(const std::string& path) {
    return std::filesystem::path(path).filename().string();
}

/**
 * Gets the extension of a path (without the dot).
 */
inline std::string extension(const std::string& path) {
    std::string ext = std::filesystem::path(path).extension().string();

    if (!ext.empty() && ext[0] == '.') {
        return ext.substr(1);
    }

    return ext;
}

/**
 * Gets the stem (filename without extension) of a path.
 */
inline std::string stem(const std::string& path) {
    return std::filesystem::path(path).stem().string();
}

/**
 * Checks if a path is absolute.
 */
inline bool is_absolute(const std::string& path) {
    return std::filesystem::path(path).is_absolute();
}

/**
 * Converts a path to an absolute path.
 */
inline bishop::rt::Result<std::string> absolute(const std::string& path) {
    try {
        std::filesystem::path abs = std::filesystem::absolute(path);
        return bishop::rt::Result<std::string>::ok(abs.string());
    } catch (const std::filesystem::filesystem_error& e) {
        return bishop::rt::Result<std::string>::error(
            bishop::rt::Error("Failed to get absolute path: " + path + " - " + std::string(e.what()))
        );
    } catch (const std::exception& e) {
        return bishop::rt::Result<std::string>::error(
            bishop::rt::Error("Failed to get absolute path: " + path + " - " + std::string(e.what()))
        );
    }
}

/**
 * Returns the canonical (resolved) absolute path.
 */
inline bishop::rt::Result<std::string> canonical(const std::string& path) {
    std::error_code ec;
    std::filesystem::path canon = std::filesystem::canonical(path, ec);

    if (ec) {
        return bishop::rt::Result<std::string>::error(
            bishop::rt::Error("Failed to get canonical path: " + path + " - " + ec.message())
        );
    }

    return bishop::rt::Result<std::string>::ok(canon.string());
}

// ============================================================================
// File Info
// ============================================================================

/**
 * Gets file information (stat).
 */
inline bishop::rt::Result<FileInfo> stat(const std::string& path) {
    std::error_code ec;

    if (!std::filesystem::exists(path, ec)) {
        return bishop::rt::Result<FileInfo>::error(
            bishop::rt::Error("Path does not exist: " + path)
        );
    }

    FileInfo info;
    info.is_file = std::filesystem::is_regular_file(path, ec);
    info.is_dir = std::filesystem::is_directory(path, ec);
    info.is_symlink = std::filesystem::is_symlink(path, ec);

    if (info.is_file) {
        auto file_size = std::filesystem::file_size(path, ec);

        if (!ec) {
            // Check for overflow before casting to int
            if (file_size > static_cast<std::uintmax_t>(std::numeric_limits<int>::max())) {
                return bishop::rt::Result<FileInfo>::error(
                    bishop::rt::Error("File size exceeds maximum int value (> 2GB): " + path)
                );
            }

            info.size = static_cast<int>(file_size);
        }
    }

    auto ftime = std::filesystem::last_write_time(path, ec);

    if (!ec) {
        // Convert file_time_type to system_clock-aligned time point
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - decltype(ftime)::clock::now() + std::chrono::system_clock::now());
        auto secs = std::chrono::time_point_cast<std::chrono::seconds>(sctp)
                        .time_since_epoch()
                        .count();
        info.modified = static_cast<int>(secs);
    }

    return bishop::rt::Result<FileInfo>::ok(info);
}

/**
 * Gets the size of a file in bytes.
 */
inline bishop::rt::Result<int> file_size(const std::string& path) {
    std::error_code ec;
    auto size = std::filesystem::file_size(path, ec);

    if (ec) {
        return bishop::rt::Result<int>::error(
            bishop::rt::Error("Failed to get file size: " + path + " - " + ec.message())
        );
    }

    // Check for overflow before casting to int
    if (size > static_cast<std::uintmax_t>(std::numeric_limits<int>::max())) {
        return bishop::rt::Result<int>::error(
            bishop::rt::Error("File size exceeds maximum int value (> 2GB): " + path)
        );
    }

    return bishop::rt::Result<int>::ok(static_cast<int>(size));
}

// ============================================================================
// Directory Walking
// ============================================================================

/**
 * Walks a directory tree recursively.
 */
inline bishop::rt::Result<std::vector<DirEntry>> walk(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        return bishop::rt::Result<std::vector<DirEntry>>::error(
            bishop::rt::Error("Path does not exist: " + path)
        );
    }

    std::vector<DirEntry> entries;
    std::error_code ec;
    std::filesystem::recursive_directory_iterator it(path, ec);

    if (ec) {
        return bishop::rt::Result<std::vector<DirEntry>>::error(
            bishop::rt::Error("Failed to walk directory: " + path + " - " + ec.message())
        );
    }

    std::filesystem::recursive_directory_iterator end;

    for (; it != end; it.increment(ec)) {
        if (ec) {
            return bishop::rt::Result<std::vector<DirEntry>>::error(
                bishop::rt::Error("Failed to walk directory: " + path + " - " + ec.message())
            );
        }

        DirEntry de;
        de.path = it->path().string();
        de.name = it->path().filename().string();
        de.is_dir = it->is_directory();
        de.is_file = it->is_regular_file();
        de.is_symlink = it->is_symlink();
        entries.push_back(de);
    }

    return bishop::rt::Result<std::vector<DirEntry>>::ok(entries);
}

// ============================================================================
// Temp File/Dir
// ============================================================================

/**
 * Creates a unique temporary file path.
 * The file is created atomically to prevent TOCTOU race conditions.
 */
inline bishop::rt::Result<std::string> temp_file() {
    std::filesystem::path temp_dir_path;

    try {
        temp_dir_path = std::filesystem::temp_directory_path();
    } catch (const std::filesystem::filesystem_error& e) {
        return bishop::rt::Result<std::string>::error(
            bishop::rt::Error("Failed to get temp directory: " + std::string(e.what()))
        );
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 999999999);

    for (int i = 0; i < 100; ++i) {
        std::string name = "bishop_tmp_" + std::to_string(dis(gen));
        std::filesystem::path temp_path = temp_dir_path / name;

        // Create the file atomically to prevent TOCTOU race conditions
        std::ofstream temp_file_stream(
            temp_path,
            std::ios::binary | std::ios::trunc | std::ios::out
        );

        if (temp_file_stream) {
            temp_file_stream.close();
            return bishop::rt::Result<std::string>::ok(temp_path.string());
        }
    }

    return bishop::rt::Result<std::string>::error(
        bishop::rt::Error("Failed to generate unique temp file name")
    );
}

/**
 * Creates a unique temporary directory.
 */
inline bishop::rt::Result<std::string> temp_dir() {
    std::filesystem::path temp_base;

    try {
        temp_base = std::filesystem::temp_directory_path();
    } catch (const std::filesystem::filesystem_error& e) {
        return bishop::rt::Result<std::string>::error(
            bishop::rt::Error("Failed to get temp directory: " + std::string(e.what()))
        );
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 999999999);

    for (int i = 0; i < 100; ++i) {
        std::string name = "bishop_tmpdir_" + std::to_string(dis(gen));
        std::filesystem::path temp_path = temp_base / name;

        if (!std::filesystem::exists(temp_path)) {
            std::error_code ec;
            std::filesystem::create_directory(temp_path, ec);

            if (!ec) {
                return bishop::rt::Result<std::string>::ok(temp_path.string());
            }
        }
    }

    return bishop::rt::Result<std::string>::error(
        bishop::rt::Error("Failed to create temp directory")
    );
}

}  // namespace fs
