/**
 * @file fs.cpp
 * @brief Built-in fs (filesystem) module implementation.
 *
 * Creates the AST definitions for the fs module.
 * The actual runtime is in src/runtime/fs/fs.hpp and included as a header.
 */

/**
 * @nog_fn read_file
 * @module fs
 * @description Reads the entire contents of a file as a string.
 * @param path str - Path to the file to read
 * @returns str - File contents, or empty string if file cannot be read
 * @example
 * import fs;
 * content := fs.read_file("config.txt");
 */

/**
 * @nog_fn exists
 * @module fs
 * @description Checks if a file or directory exists at the given path.
 * @param path str - Path to check
 * @returns bool - True if path exists, false otherwise
 * @example
 * import fs;
 * if fs.exists("data.json") {
 *     content := fs.read_file("data.json");
 * }
 */

/**
 * @nog_fn is_dir
 * @module fs
 * @description Checks if the given path is a directory.
 * @param path str - Path to check
 * @returns bool - True if path is a directory, false otherwise
 * @example
 * import fs;
 * if fs.is_dir("./uploads") {
 *     files := fs.list_dir("./uploads");
 * }
 */

/**
 * @nog_fn is_file
 * @module fs
 * @description Checks if the given path is a regular file.
 * @param path str - Path to check
 * @returns bool - True if path is a regular file, false otherwise
 * @example
 * import fs;
 * if fs.is_file("data.txt") {
 *     content := fs.read_file("data.txt");
 * }
 */

/**
 * @nog_fn read_dir
 * @module fs
 * @description Lists all entries in a directory, separated by newlines.
 * @param path str - Path to the directory
 * @returns str - Newline-separated list of filenames
 * @example
 * import fs;
 * files := fs.read_dir("./data");
 */

/**
 * @nog_fn write_file
 * @module fs
 * @description Writes content to a file, creating it if it doesn't exist.
 * @param path str - Path to the file
 * @param content str - Content to write
 * @returns bool or err - True on success, error on failure
 * @example
 * import fs;
 * fs.write_file("output.txt", "Hello World") or fail err;
 */

/**
 * @nog_fn append_file
 * @module fs
 * @description Appends content to a file, creating it if it doesn't exist.
 * @param path str - Path to the file
 * @param content str - Content to append
 * @returns bool or err - True on success, error on failure
 * @example
 * import fs;
 * fs.append_file("log.txt", "New log entry\n") or fail err;
 */

/**
 * @nog_fn read_bytes
 * @module fs
 * @description Reads binary content from a file.
 * @param path str - Path to the file
 * @returns str or err - Binary content as string, error on failure
 * @example
 * import fs;
 * data := fs.read_bytes("image.png") or fail err;
 */

/**
 * @nog_fn write_bytes
 * @module fs
 * @description Writes binary content to a file.
 * @param path str - Path to the file
 * @param data str - Binary data to write
 * @returns bool or err - True on success, error on failure
 * @example
 * import fs;
 * fs.write_bytes("output.bin", data) or fail err;
 */

/**
 * @nog_fn open
 * @module fs
 * @description Opens a file with the specified mode.
 * @param path str - Path to the file
 * @param mode str - Mode: "r" (read), "w" (write), "a" (append), "rw" (read+write)
 * @returns fs.File or err - File handle or error
 * @example
 * import fs;
 * with fs.open("data.txt", "r") as f {
 *     line := f.read_line() or fail err;
 *     print(line);
 * }
 */

/**
 * @nog_fn list_dir
 * @module fs
 * @description Lists all entries in a directory as a List<str>.
 * @param path str - Path to the directory
 * @returns List<str> or err - List of filenames or error
 * @example
 * import fs;
 * files := fs.list_dir(".") or fail err;
 * for file in files {
 *     print(file);
 * }
 */

/**
 * @nog_fn mkdir
 * @module fs
 * @description Creates a single directory.
 * @param path str - Path to create
 * @returns bool or err - True on success, error on failure
 * @example
 * import fs;
 * fs.mkdir("new_dir") or fail err;
 */

/**
 * @nog_fn mkdir_all
 * @module fs
 * @description Creates a directory and all parent directories.
 * @param path str - Path to create
 * @returns bool or err - True on success, error on failure
 * @example
 * import fs;
 * fs.mkdir_all("path/to/deep/dir") or fail err;
 */

/**
 * @nog_fn remove
 * @module fs
 * @description Removes a single file.
 * @param path str - Path to the file
 * @returns bool or err - True on success, error on failure
 * @example
 * import fs;
 * fs.remove("old_file.txt") or fail err;
 */

/**
 * @nog_fn remove_dir
 * @module fs
 * @description Removes an empty directory.
 * @param path str - Path to the directory
 * @returns bool or err - True on success, error on failure
 * @example
 * import fs;
 * fs.remove_dir("empty_dir") or fail err;
 */

/**
 * @nog_fn remove_all
 * @module fs
 * @description Removes a file or directory and all its contents.
 * @param path str - Path to remove
 * @returns bool or err - True on success, error on failure
 * @example
 * import fs;
 * fs.remove_all("dir_with_contents") or fail err;
 */

/**
 * @nog_fn rename
 * @module fs
 * @description Renames or moves a file or directory.
 * @param from str - Source path
 * @param to str - Destination path
 * @returns bool or err - True on success, error on failure
 * @example
 * import fs;
 * fs.rename("old_name.txt", "new_name.txt") or fail err;
 */

/**
 * @nog_fn copy
 * @module fs
 * @description Copies a single file.
 * @param from str - Source path
 * @param to str - Destination path
 * @returns bool or err - True on success, error on failure
 * @example
 * import fs;
 * fs.copy("source.txt", "destination.txt") or fail err;
 */

/**
 * @nog_fn copy_dir
 * @module fs
 * @description Copies a directory and all its contents.
 * @param from str - Source path
 * @param to str - Destination path
 * @returns bool or err - True on success, error on failure
 * @example
 * import fs;
 * fs.copy_dir("source_dir", "dest_dir") or fail err;
 */

/**
 * @nog_fn join
 * @module fs
 * @description Joins two path components together.
 * @param a str - First path component
 * @param b str - Second path component
 * @returns str - Joined path
 * @example
 * import fs;
 * path := fs.join(fs.join("dir", "subdir"), "file.txt");
 */

/**
 * @nog_fn dirname
 * @module fs
 * @description Gets the directory part of a path.
 * @param path str - Path to get directory from
 * @returns str - Directory part of path
 * @example
 * import fs;
 * dir := fs.dirname("/home/user/file.txt");  // "/home/user"
 */

/**
 * @nog_fn basename
 * @module fs
 * @description Gets the filename part of a path.
 * @param path str - Path to get filename from
 * @returns str - Filename part of path
 * @example
 * import fs;
 * name := fs.basename("/home/user/file.txt");  // "file.txt"
 */

/**
 * @nog_fn extension
 * @module fs
 * @description Gets the extension of a path (without the dot).
 * @param path str - Path to get extension from
 * @returns str - Extension without dot
 * @example
 * import fs;
 * ext := fs.extension("file.txt");  // "txt"
 */

/**
 * @nog_fn stem
 * @module fs
 * @description Gets the stem (filename without extension) of a path.
 * @param path str - Path to get stem from
 * @returns str - Stem of the path
 * @example
 * import fs;
 * name := fs.stem("file.txt");  // "file"
 */

/**
 * @nog_fn is_absolute
 * @module fs
 * @description Checks if a path is absolute.
 * @param path str - Path to check
 * @returns bool - True if path is absolute
 * @example
 * import fs;
 * if fs.is_absolute("/home/user") {
 *     print("Absolute path");
 * }
 */

/**
 * @nog_fn absolute
 * @module fs
 * @description Converts a path to an absolute path.
 * @param path str - Path to convert
 * @returns str or err - Absolute path or error
 * @example
 * import fs;
 * abs := fs.absolute("relative/path") or fail err;
 */

/**
 * @nog_fn canonical
 * @module fs
 * @description Returns the canonical (resolved) absolute path.
 * @param path str - Path to resolve
 * @returns str or err - Canonical path or error
 * @example
 * import fs;
 * canon := fs.canonical("./some/../path") or fail err;
 */

/**
 * @nog_fn stat
 * @module fs
 * @description Gets file information.
 * @param path str - Path to get info for
 * @returns fs.FileInfo or err - File info struct or error
 * @example
 * import fs;
 * info := fs.stat("file.txt") or fail err;
 * print(info.size);
 * print(info.is_file);
 */

/**
 * @nog_fn file_size
 * @module fs
 * @description Gets the size of a file in bytes.
 * @param path str - Path to the file
 * @returns int or err - File size in bytes or error
 * @example
 * import fs;
 * size := fs.file_size("file.txt") or fail err;
 */

/**
 * @nog_fn walk
 * @module fs
 * @description Walks a directory tree recursively.
 * @param path str - Path to walk
 * @returns List<fs.DirEntry> or err - List of directory entries or error
 * @example
 * import fs;
 * for entry in fs.walk("src") or fail err {
 *     if !entry.is_dir {
 *         print(entry.path);
 *     }
 * }
 */

/**
 * @nog_fn temp_file
 * @module fs
 * @description Creates a unique temporary file path.
 * @returns str or err - Temporary file path or error
 * @example
 * import fs;
 * path := fs.temp_file() or fail err;
 * fs.write_file(path, "temp content") or fail err;
 */

/**
 * @nog_fn temp_dir
 * @module fs
 * @description Creates a unique temporary directory.
 * @returns str or err - Temporary directory path or error
 * @example
 * import fs;
 * path := fs.temp_dir() or fail err;
 */

/**
 * @nog_struct FileInfo
 * @module fs
 * @description File information returned by stat().
 * @field size int - File size in bytes
 * @field modified int - Last modification time (Unix timestamp)
 * @field created int - Creation time (Unix timestamp)
 * @field accessed int - Last access time (Unix timestamp)
 * @field is_file bool - True if path is a regular file
 * @field is_dir bool - True if path is a directory
 * @field is_symlink bool - True if path is a symbolic link
 */

/**
 * @nog_struct DirEntry
 * @module fs
 * @description Directory entry returned by walk().
 * @field path str - Full path to the entry
 * @field name str - Filename part of the path
 * @field is_dir bool - True if entry is a directory
 * @field is_file bool - True if entry is a regular file
 * @field is_symlink bool - True if entry is a symbolic link
 */

/**
 * @nog_struct File
 * @module fs
 * @description File handle for reading and writing.
 * Supports the `with` statement for automatic resource cleanup.
 */

/**
 * @nog_method read_line
 * @type fs.File
 * @description Reads a single line from the file.
 * @returns str or err - Line content or error
 */

/**
 * @nog_method read_lines
 * @type fs.File
 * @description Reads all lines from the file.
 * @returns List<str> or err - List of lines or error
 */

/**
 * @nog_method read_all
 * @type fs.File
 * @description Reads all content from the file.
 * @returns str or err - File content or error
 */

/**
 * @nog_method write
 * @type fs.File
 * @description Writes data to the file.
 * @param data str - Data to write
 * @returns bool or err - True on success or error
 */

/**
 * @nog_method write_line
 * @type fs.File
 * @description Writes data followed by a newline to the file.
 * @param data str - Data to write
 * @returns bool or err - True on success or error
 */

/**
 * @nog_method close
 * @type fs.File
 * @description Closes the file.
 */

#include "fs.hpp"

using namespace std;

namespace nog::stdlib {

/**
 * Creates the AST for the built-in fs module.
 */
unique_ptr<Program> create_fs_module() {
    auto program = make_unique<Program>();

    // ========================================================================
    // Structs
    // ========================================================================

    // FileInfo :: struct { size int, modified int, created int, accessed int,
    //                      is_file bool, is_dir bool, is_symlink bool }
    auto fileinfo_struct = make_unique<StructDef>();
    fileinfo_struct->name = "FileInfo";
    fileinfo_struct->visibility = Visibility::Public;
    fileinfo_struct->fields.push_back({"size", "int", ""});
    fileinfo_struct->fields.push_back({"modified", "int", ""});
    fileinfo_struct->fields.push_back({"created", "int", ""});
    fileinfo_struct->fields.push_back({"accessed", "int", ""});
    fileinfo_struct->fields.push_back({"is_file", "bool", ""});
    fileinfo_struct->fields.push_back({"is_dir", "bool", ""});
    fileinfo_struct->fields.push_back({"is_symlink", "bool", ""});
    program->structs.push_back(move(fileinfo_struct));

    // DirEntry :: struct { path str, name str, is_dir bool, is_file bool, is_symlink bool }
    auto direntry_struct = make_unique<StructDef>();
    direntry_struct->name = "DirEntry";
    direntry_struct->visibility = Visibility::Public;
    direntry_struct->fields.push_back({"path", "str", ""});
    direntry_struct->fields.push_back({"name", "str", ""});
    direntry_struct->fields.push_back({"is_dir", "bool", ""});
    direntry_struct->fields.push_back({"is_file", "bool", ""});
    direntry_struct->fields.push_back({"is_symlink", "bool", ""});
    program->structs.push_back(move(direntry_struct));

    // File :: struct { } (opaque handle)
    auto file_struct = make_unique<StructDef>();
    file_struct->name = "File";
    file_struct->visibility = Visibility::Public;
    program->structs.push_back(move(file_struct));

    // ========================================================================
    // Existing Functions
    // ========================================================================

    // fn read_file(str path) -> str
    auto read_file_fn = make_unique<FunctionDef>();
    read_file_fn->name = "read_file";
    read_file_fn->visibility = Visibility::Public;
    read_file_fn->params.push_back({"str", "path"});
    read_file_fn->return_type = "str";
    program->functions.push_back(move(read_file_fn));

    // fn exists(str path) -> bool
    auto exists_fn = make_unique<FunctionDef>();
    exists_fn->name = "exists";
    exists_fn->visibility = Visibility::Public;
    exists_fn->params.push_back({"str", "path"});
    exists_fn->return_type = "bool";
    program->functions.push_back(move(exists_fn));

    // fn is_dir(str path) -> bool
    auto is_dir_fn = make_unique<FunctionDef>();
    is_dir_fn->name = "is_dir";
    is_dir_fn->visibility = Visibility::Public;
    is_dir_fn->params.push_back({"str", "path"});
    is_dir_fn->return_type = "bool";
    program->functions.push_back(move(is_dir_fn));

    // fn is_file(str path) -> bool
    auto is_file_fn = make_unique<FunctionDef>();
    is_file_fn->name = "is_file";
    is_file_fn->visibility = Visibility::Public;
    is_file_fn->params.push_back({"str", "path"});
    is_file_fn->return_type = "bool";
    program->functions.push_back(move(is_file_fn));

    // fn read_dir(str path) -> str
    auto read_dir_fn = make_unique<FunctionDef>();
    read_dir_fn->name = "read_dir";
    read_dir_fn->visibility = Visibility::Public;
    read_dir_fn->params.push_back({"str", "path"});
    read_dir_fn->return_type = "str";
    program->functions.push_back(move(read_dir_fn));

    // ========================================================================
    // File Writing Functions
    // ========================================================================

    // fn write_file(str path, str content) -> bool or err
    auto write_file_fn = make_unique<FunctionDef>();
    write_file_fn->name = "write_file";
    write_file_fn->visibility = Visibility::Public;
    write_file_fn->params.push_back({"str", "path"});
    write_file_fn->params.push_back({"str", "content"});
    write_file_fn->return_type = "bool";
    write_file_fn->error_type = "err";
    program->functions.push_back(move(write_file_fn));

    // fn append_file(str path, str content) -> bool or err
    auto append_file_fn = make_unique<FunctionDef>();
    append_file_fn->name = "append_file";
    append_file_fn->visibility = Visibility::Public;
    append_file_fn->params.push_back({"str", "path"});
    append_file_fn->params.push_back({"str", "content"});
    append_file_fn->return_type = "bool";
    append_file_fn->error_type = "err";
    program->functions.push_back(move(append_file_fn));

    // fn read_bytes(str path) -> str or err
    auto read_bytes_fn = make_unique<FunctionDef>();
    read_bytes_fn->name = "read_bytes";
    read_bytes_fn->visibility = Visibility::Public;
    read_bytes_fn->params.push_back({"str", "path"});
    read_bytes_fn->return_type = "str";
    read_bytes_fn->error_type = "err";
    program->functions.push_back(move(read_bytes_fn));

    // fn write_bytes(str path, str data) -> bool or err
    auto write_bytes_fn = make_unique<FunctionDef>();
    write_bytes_fn->name = "write_bytes";
    write_bytes_fn->visibility = Visibility::Public;
    write_bytes_fn->params.push_back({"str", "path"});
    write_bytes_fn->params.push_back({"str", "data"});
    write_bytes_fn->return_type = "bool";
    write_bytes_fn->error_type = "err";
    program->functions.push_back(move(write_bytes_fn));

    // ========================================================================
    // File Handle Functions
    // ========================================================================

    // fn open(str path, str mode) -> fs.File or err
    auto open_fn = make_unique<FunctionDef>();
    open_fn->name = "open";
    open_fn->visibility = Visibility::Public;
    open_fn->params.push_back({"str", "path"});
    open_fn->params.push_back({"str", "mode"});
    open_fn->return_type = "fs.File";
    open_fn->error_type = "err";
    program->functions.push_back(move(open_fn));

    // ========================================================================
    // Directory Operations
    // ========================================================================

    // fn list_dir(str path) -> List<str> or err
    auto list_dir_fn = make_unique<FunctionDef>();
    list_dir_fn->name = "list_dir";
    list_dir_fn->visibility = Visibility::Public;
    list_dir_fn->params.push_back({"str", "path"});
    list_dir_fn->return_type = "List<str>";
    list_dir_fn->error_type = "err";
    program->functions.push_back(move(list_dir_fn));

    // fn mkdir(str path) -> bool or err
    auto mkdir_fn = make_unique<FunctionDef>();
    mkdir_fn->name = "mkdir";
    mkdir_fn->visibility = Visibility::Public;
    mkdir_fn->params.push_back({"str", "path"});
    mkdir_fn->return_type = "bool";
    mkdir_fn->error_type = "err";
    program->functions.push_back(move(mkdir_fn));

    // fn mkdir_all(str path) -> bool or err
    auto mkdir_all_fn = make_unique<FunctionDef>();
    mkdir_all_fn->name = "mkdir_all";
    mkdir_all_fn->visibility = Visibility::Public;
    mkdir_all_fn->params.push_back({"str", "path"});
    mkdir_all_fn->return_type = "bool";
    mkdir_all_fn->error_type = "err";
    program->functions.push_back(move(mkdir_all_fn));

    // fn remove(str path) -> bool or err
    auto remove_fn = make_unique<FunctionDef>();
    remove_fn->name = "remove";
    remove_fn->visibility = Visibility::Public;
    remove_fn->params.push_back({"str", "path"});
    remove_fn->return_type = "bool";
    remove_fn->error_type = "err";
    program->functions.push_back(move(remove_fn));

    // fn remove_dir(str path) -> bool or err
    auto remove_dir_fn = make_unique<FunctionDef>();
    remove_dir_fn->name = "remove_dir";
    remove_dir_fn->visibility = Visibility::Public;
    remove_dir_fn->params.push_back({"str", "path"});
    remove_dir_fn->return_type = "bool";
    remove_dir_fn->error_type = "err";
    program->functions.push_back(move(remove_dir_fn));

    // fn remove_all(str path) -> bool or err
    auto remove_all_fn = make_unique<FunctionDef>();
    remove_all_fn->name = "remove_all";
    remove_all_fn->visibility = Visibility::Public;
    remove_all_fn->params.push_back({"str", "path"});
    remove_all_fn->return_type = "bool";
    remove_all_fn->error_type = "err";
    program->functions.push_back(move(remove_all_fn));

    // ========================================================================
    // File Operations
    // ========================================================================

    // fn rename(str from, str to) -> bool or err
    auto rename_fn = make_unique<FunctionDef>();
    rename_fn->name = "rename";
    rename_fn->visibility = Visibility::Public;
    rename_fn->params.push_back({"str", "from"});
    rename_fn->params.push_back({"str", "to"});
    rename_fn->return_type = "bool";
    rename_fn->error_type = "err";
    program->functions.push_back(move(rename_fn));

    // fn copy(str from, str to) -> bool or err
    auto copy_fn = make_unique<FunctionDef>();
    copy_fn->name = "copy";
    copy_fn->visibility = Visibility::Public;
    copy_fn->params.push_back({"str", "from"});
    copy_fn->params.push_back({"str", "to"});
    copy_fn->return_type = "bool";
    copy_fn->error_type = "err";
    program->functions.push_back(move(copy_fn));

    // fn copy_dir(str from, str to) -> bool or err
    auto copy_dir_fn = make_unique<FunctionDef>();
    copy_dir_fn->name = "copy_dir";
    copy_dir_fn->visibility = Visibility::Public;
    copy_dir_fn->params.push_back({"str", "from"});
    copy_dir_fn->params.push_back({"str", "to"});
    copy_dir_fn->return_type = "bool";
    copy_dir_fn->error_type = "err";
    program->functions.push_back(move(copy_dir_fn));

    // ========================================================================
    // Path Operations
    // ========================================================================

    // fn join(str a, str b) -> str
    auto join_fn = make_unique<FunctionDef>();
    join_fn->name = "join";
    join_fn->visibility = Visibility::Public;
    join_fn->params.push_back({"str", "a"});
    join_fn->params.push_back({"str", "b"});
    join_fn->return_type = "str";
    program->functions.push_back(move(join_fn));

    // fn dirname(str path) -> str
    auto dirname_fn = make_unique<FunctionDef>();
    dirname_fn->name = "dirname";
    dirname_fn->visibility = Visibility::Public;
    dirname_fn->params.push_back({"str", "path"});
    dirname_fn->return_type = "str";
    program->functions.push_back(move(dirname_fn));

    // fn basename(str path) -> str
    auto basename_fn = make_unique<FunctionDef>();
    basename_fn->name = "basename";
    basename_fn->visibility = Visibility::Public;
    basename_fn->params.push_back({"str", "path"});
    basename_fn->return_type = "str";
    program->functions.push_back(move(basename_fn));

    // fn extension(str path) -> str
    auto extension_fn = make_unique<FunctionDef>();
    extension_fn->name = "extension";
    extension_fn->visibility = Visibility::Public;
    extension_fn->params.push_back({"str", "path"});
    extension_fn->return_type = "str";
    program->functions.push_back(move(extension_fn));

    // fn stem(str path) -> str
    auto stem_fn = make_unique<FunctionDef>();
    stem_fn->name = "stem";
    stem_fn->visibility = Visibility::Public;
    stem_fn->params.push_back({"str", "path"});
    stem_fn->return_type = "str";
    program->functions.push_back(move(stem_fn));

    // fn is_absolute(str path) -> bool
    auto is_absolute_fn = make_unique<FunctionDef>();
    is_absolute_fn->name = "is_absolute";
    is_absolute_fn->visibility = Visibility::Public;
    is_absolute_fn->params.push_back({"str", "path"});
    is_absolute_fn->return_type = "bool";
    program->functions.push_back(move(is_absolute_fn));

    // fn absolute(str path) -> str or err
    auto absolute_fn = make_unique<FunctionDef>();
    absolute_fn->name = "absolute";
    absolute_fn->visibility = Visibility::Public;
    absolute_fn->params.push_back({"str", "path"});
    absolute_fn->return_type = "str";
    absolute_fn->error_type = "err";
    program->functions.push_back(move(absolute_fn));

    // fn canonical(str path) -> str or err
    auto canonical_fn = make_unique<FunctionDef>();
    canonical_fn->name = "canonical";
    canonical_fn->visibility = Visibility::Public;
    canonical_fn->params.push_back({"str", "path"});
    canonical_fn->return_type = "str";
    canonical_fn->error_type = "err";
    program->functions.push_back(move(canonical_fn));

    // ========================================================================
    // File Info
    // ========================================================================

    // fn stat(str path) -> fs.FileInfo or err
    auto stat_fn = make_unique<FunctionDef>();
    stat_fn->name = "stat";
    stat_fn->visibility = Visibility::Public;
    stat_fn->params.push_back({"str", "path"});
    stat_fn->return_type = "fs.FileInfo";
    stat_fn->error_type = "err";
    program->functions.push_back(move(stat_fn));

    // fn file_size(str path) -> int or err
    auto file_size_fn = make_unique<FunctionDef>();
    file_size_fn->name = "file_size";
    file_size_fn->visibility = Visibility::Public;
    file_size_fn->params.push_back({"str", "path"});
    file_size_fn->return_type = "int";
    file_size_fn->error_type = "err";
    program->functions.push_back(move(file_size_fn));

    // ========================================================================
    // Directory Walking
    // ========================================================================

    // fn walk(str path) -> List<fs.DirEntry> or err
    auto walk_fn = make_unique<FunctionDef>();
    walk_fn->name = "walk";
    walk_fn->visibility = Visibility::Public;
    walk_fn->params.push_back({"str", "path"});
    walk_fn->return_type = "List<fs.DirEntry>";
    walk_fn->error_type = "err";
    program->functions.push_back(move(walk_fn));

    // ========================================================================
    // Temp File/Dir
    // ========================================================================

    // fn temp_file() -> str or err
    auto temp_file_fn = make_unique<FunctionDef>();
    temp_file_fn->name = "temp_file";
    temp_file_fn->visibility = Visibility::Public;
    temp_file_fn->return_type = "str";
    temp_file_fn->error_type = "err";
    program->functions.push_back(move(temp_file_fn));

    // fn temp_dir() -> str or err
    auto temp_dir_fn = make_unique<FunctionDef>();
    temp_dir_fn->name = "temp_dir";
    temp_dir_fn->visibility = Visibility::Public;
    temp_dir_fn->return_type = "str";
    temp_dir_fn->error_type = "err";
    program->functions.push_back(move(temp_dir_fn));

    // ========================================================================
    // File Handle Methods
    // ========================================================================

    // File :: read_line(self) -> str or err
    auto read_line_method = make_unique<MethodDef>();
    read_line_method->struct_name = "File";
    read_line_method->name = "read_line";
    read_line_method->visibility = Visibility::Public;
    read_line_method->params.push_back({"fs.File", "self"});
    read_line_method->return_type = "str";
    read_line_method->error_type = "err";
    program->methods.push_back(move(read_line_method));

    // File :: read_lines(self) -> List<str> or err
    auto read_lines_method = make_unique<MethodDef>();
    read_lines_method->struct_name = "File";
    read_lines_method->name = "read_lines";
    read_lines_method->visibility = Visibility::Public;
    read_lines_method->params.push_back({"fs.File", "self"});
    read_lines_method->return_type = "List<str>";
    read_lines_method->error_type = "err";
    program->methods.push_back(move(read_lines_method));

    // File :: read_all(self) -> str or err
    auto read_all_method = make_unique<MethodDef>();
    read_all_method->struct_name = "File";
    read_all_method->name = "read_all";
    read_all_method->visibility = Visibility::Public;
    read_all_method->params.push_back({"fs.File", "self"});
    read_all_method->return_type = "str";
    read_all_method->error_type = "err";
    program->methods.push_back(move(read_all_method));

    // File :: write(self, str data) -> bool or err
    auto write_method = make_unique<MethodDef>();
    write_method->struct_name = "File";
    write_method->name = "write";
    write_method->visibility = Visibility::Public;
    write_method->params.push_back({"fs.File", "self"});
    write_method->params.push_back({"str", "data"});
    write_method->return_type = "bool";
    write_method->error_type = "err";
    program->methods.push_back(move(write_method));

    // File :: write_line(self, str data) -> bool or err
    auto write_line_method = make_unique<MethodDef>();
    write_line_method->struct_name = "File";
    write_line_method->name = "write_line";
    write_line_method->visibility = Visibility::Public;
    write_line_method->params.push_back({"fs.File", "self"});
    write_line_method->params.push_back({"str", "data"});
    write_line_method->return_type = "bool";
    write_line_method->error_type = "err";
    program->methods.push_back(move(write_line_method));

    // File :: close(self)
    auto close_method = make_unique<MethodDef>();
    close_method->struct_name = "File";
    close_method->name = "close";
    close_method->visibility = Visibility::Public;
    close_method->params.push_back({"fs.File", "self"});
    program->methods.push_back(move(close_method));

    return program;
}

/**
 * Returns empty - fs.hpp is included at the top of generated code
 * for precompiled header support.
 */
string generate_fs_runtime() {
    return "";
}

}  // namespace nog::stdlib
