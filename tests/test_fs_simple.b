// Simple fs test

import fs;

fn test_read() {
    content := fs.read_file("tests/test_fs_simple.b");
    assert_eq(true, content.length() > 0);
}

fn test_exists() {
    assert_eq(true, fs.exists("tests"));
}

fn test_write() or err {
    _a := fs.write_file("test_out.txt", "Hello") or fail err;
    content := fs.read_file("test_out.txt");
    assert_eq("Hello", content);
    _b := fs.remove("test_out.txt") or fail err;
}
