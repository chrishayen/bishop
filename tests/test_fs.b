// Tests for the fs module

import fs;

// ============================================================================
// File Reading Tests (existing)
// ============================================================================

fn test_read_file() {
    content := fs.read_file("tests/test_fs.b");
    assert_eq(true, content.length() > 0);
}

fn test_read_file_nonexistent() {
    content := fs.read_file("nonexistent_file_12345.txt");
    assert_eq(0, content.length());
}

// ============================================================================
// File Existence Tests (existing)
// ============================================================================

fn test_exists_file() {
    assert_eq(true, fs.exists("tests/test_fs.b"));
}

fn test_exists_dir() {
    assert_eq(true, fs.exists("tests"));
}

fn test_exists_nonexistent() {
    assert_eq(false, fs.exists("nonexistent_file_12345.txt"));
}

fn test_is_dir_true() {
    assert_eq(true, fs.is_dir("tests"));
}

fn test_is_dir_false() {
    assert_eq(false, fs.is_dir("tests/test_fs.b"));
}

fn test_read_dir() {
    files := fs.read_dir("tests");
    assert_eq(true, files.length() > 0);
}

// ============================================================================
// File Writing Tests
// ============================================================================

fn test_write_file() or err {
    _w := fs.write_file("test_output.txt", "Hello World") or fail err;
    content := fs.read_file("test_output.txt");
    assert_eq("Hello World", content);
    _r := fs.remove("test_output.txt") or fail err;
}

fn test_append_file() or err {
    _w1 := fs.write_file("test_append.txt", "First") or fail err;
    _a := fs.append_file("test_append.txt", "\nSecond") or fail err;
    content := fs.read_file("test_append.txt");
    assert_eq("First\nSecond", content);
    _r := fs.remove("test_append.txt") or fail err;
}

fn test_write_bytes() or err {
    _w := fs.write_bytes("test_bytes.bin", "binary data") or fail err;
    data := fs.read_bytes("test_bytes.bin") or fail err;
    assert_eq("binary data", data);
    _r := fs.remove("test_bytes.bin") or fail err;
}

// ============================================================================
// File Handle Tests
// Note: Tests file handle open and basic operations
// ============================================================================

fn test_file_handle_open_close() or err {
    _w := fs.write_file("test_handle.txt", "Test content") or fail err;

    f := fs.open("test_handle.txt", "r") or fail err;
    f.close();

    _r := fs.remove("test_handle.txt") or fail err;
}

// ============================================================================
// Directory Operations Tests
// ============================================================================

fn test_list_dir() or err {
    files := fs.list_dir("tests") or fail err;
    assert_eq(true, files.length() > 0);
}

fn test_mkdir_and_remove_dir() or err {
    _m := fs.mkdir("test_single_dir") or fail err;
    assert_eq(true, fs.exists("test_single_dir"));
    assert_eq(true, fs.is_dir("test_single_dir"));
    _r := fs.remove_dir("test_single_dir") or fail err;
    assert_eq(false, fs.exists("test_single_dir"));
}

fn test_mkdir_all() or err {
    _m := fs.mkdir_all("test_nested/path/to/dir") or fail err;
    assert_eq(true, fs.exists("test_nested/path/to/dir"));
    assert_eq(true, fs.is_dir("test_nested/path/to/dir"));
    _r := fs.remove_all("test_nested") or fail err;
    assert_eq(false, fs.exists("test_nested"));
}

fn test_remove_all() or err {
    _m1 := fs.mkdir_all("test_tree/sub1/sub2") or fail err;
    _w1 := fs.write_file("test_tree/file1.txt", "content") or fail err;
    _w2 := fs.write_file("test_tree/sub1/file2.txt", "content") or fail err;
    _r := fs.remove_all("test_tree") or fail err;
    assert_eq(false, fs.exists("test_tree"));
}

// ============================================================================
// File Operations Tests
// ============================================================================

fn test_rename() or err {
    _w := fs.write_file("test_rename_src.txt", "data") or fail err;
    _m := fs.rename("test_rename_src.txt", "test_rename_dst.txt") or fail err;
    assert_eq(false, fs.exists("test_rename_src.txt"));
    assert_eq(true, fs.exists("test_rename_dst.txt"));
    content := fs.read_file("test_rename_dst.txt");
    assert_eq("data", content);
    _r := fs.remove("test_rename_dst.txt") or fail err;
}

fn test_copy() or err {
    _w := fs.write_file("test_copy_src.txt", "copy me") or fail err;
    _c := fs.copy("test_copy_src.txt", "test_copy_dst.txt") or fail err;
    assert_eq(true, fs.exists("test_copy_src.txt"));
    assert_eq(true, fs.exists("test_copy_dst.txt"));
    content := fs.read_file("test_copy_dst.txt");
    assert_eq("copy me", content);
    _r1 := fs.remove("test_copy_src.txt") or fail err;
    _r2 := fs.remove("test_copy_dst.txt") or fail err;
}

fn test_copy_dir() or err {
    _m := fs.mkdir_all("test_copydir/sub") or fail err;
    _w1 := fs.write_file("test_copydir/file.txt", "hello") or fail err;
    _w2 := fs.write_file("test_copydir/sub/nested.txt", "world") or fail err;
    _c := fs.copy_dir("test_copydir", "test_copydir_copy") or fail err;
    assert_eq(true, fs.exists("test_copydir_copy/file.txt"));
    assert_eq(true, fs.exists("test_copydir_copy/sub/nested.txt"));
    _r1 := fs.remove_all("test_copydir") or fail err;
    _r2 := fs.remove_all("test_copydir_copy") or fail err;
}

// ============================================================================
// Path Operations Tests
// ============================================================================

fn test_join() {
    path := fs.join("dir", "file.txt");
    assert_eq("dir/file.txt", path);
}

fn test_join_nested() {
    // Use nested join calls instead of 3-argument version
    path := fs.join(fs.join("a", "b"), "c");
    assert_eq("a/b/c", path);
}

fn test_dirname() {
    path := fs.dirname("/home/user/file.txt");
    assert_eq("/home/user", path);
}

fn test_basename() {
    name := fs.basename("/home/user/file.txt");
    assert_eq("file.txt", name);
}

fn test_extension() {
    ext := fs.extension("file.txt");
    assert_eq("txt", ext);
}

fn test_extension_multiple_dots() {
    ext := fs.extension("archive.tar.gz");
    assert_eq("gz", ext);
}

fn test_stem() {
    name := fs.stem("file.txt");
    assert_eq("file", name);
}

fn test_stem_multiple_dots() {
    name := fs.stem("archive.tar.gz");
    assert_eq("archive.tar", name);
}

fn test_is_absolute() {
    assert_eq(true, fs.is_absolute("/home/user"));
    assert_eq(false, fs.is_absolute("relative/path"));
}

fn test_absolute() or err {
    abs := fs.absolute("tests") or fail err;
    assert_eq(true, fs.is_absolute(abs));
    assert_eq(true, abs.contains("tests"));
}

fn test_canonical() or err {
    _m := fs.mkdir("test_canon") or fail err;
    canon := fs.canonical("test_canon") or fail err;
    assert_eq(true, fs.is_absolute(canon));
    _r := fs.remove_dir("test_canon") or fail err;
}

// ============================================================================
// File Info Tests
// ============================================================================

fn test_is_file() {
    assert_eq(true, fs.is_file("tests/test_fs.b"));
    assert_eq(false, fs.is_file("tests"));
}

fn test_stat() or err {
    info := fs.stat("tests/test_fs.b") or fail err;
    assert_eq(true, info.size > 0);
    assert_eq(true, info.is_file);
    assert_eq(false, info.is_dir);
}

fn test_stat_dir() or err {
    info := fs.stat("tests") or fail err;
    assert_eq(false, info.is_file);
    assert_eq(true, info.is_dir);
}

fn test_file_size() or err {
    _w := fs.write_file("test_size.txt", "12345") or fail err;
    size := fs.file_size("test_size.txt") or fail err;
    assert_eq(5, size);
    _r := fs.remove("test_size.txt") or fail err;
}

// ============================================================================
// Directory Walking Tests
// ============================================================================

fn test_walk() or err {
    _m1 := fs.mkdir_all("test_walk/sub1") or fail err;
    _m2 := fs.mkdir_all("test_walk/sub2") or fail err;
    _w1 := fs.write_file("test_walk/file1.txt", "a") or fail err;
    _w2 := fs.write_file("test_walk/sub1/file2.txt", "b") or fail err;

    entries := fs.walk("test_walk") or fail err;
    assert_eq(true, entries.length() >= 3);

    _r := fs.remove_all("test_walk") or fail err;
}

// ============================================================================
// Temp File/Dir Tests
// ============================================================================

fn test_temp_file() or err {
    path := fs.temp_file() or fail err;
    assert_eq(true, path.length() > 0);
    _w := fs.write_file(path, "temp content") or fail err;
    content := fs.read_file(path);
    assert_eq("temp content", content);
    _r := fs.remove(path) or fail err;
}

fn test_temp_dir() or err {
    path := fs.temp_dir() or fail err;
    assert_eq(true, path.length() > 0);
    assert_eq(true, fs.exists(path));
    assert_eq(true, fs.is_dir(path));
    _r := fs.remove_dir(path) or fail err;
}
