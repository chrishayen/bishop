/// Tests for the process module

import process;

// ============================================================
// Tests for process.run
// ============================================================

fn test_run_simple_command() or err {
    result := process.run("echo", ["hello"]) or fail err;
    assert_eq(true, result.success);
    assert_eq(0, result.exit_code);
    assert_eq(true, result.stdout.contains("hello"));
}

fn test_run_ls_command() or err {
    result := process.run("ls", ["-la"]) or fail err;
    assert_eq(true, result.success);
    assert_eq(0, result.exit_code);
    assert_eq(true, result.stdout.length() > 0);
}

fn test_run_with_multiple_args() or err {
    result := process.run("echo", ["one", "two", "three"]) or fail err;
    assert_eq(true, result.success);
    assert_eq(true, result.stdout.contains("one"));
    assert_eq(true, result.stdout.contains("two"));
    assert_eq(true, result.stdout.contains("three"));
}

fn test_run_command_not_found() or err {
    result := process.run("nonexistent_command_xyz123", []) or {
        return;
    };
    assert_eq(false, result.success);
    assert_eq(true, result.exit_code != 0);
}

fn test_run_command_exit_code() or err {
    result := process.run("false", []) or fail err;
    assert_eq(false, result.success);
    assert_eq(1, result.exit_code);
}

// ============================================================
// Tests for process.shell
// ============================================================

fn test_shell_simple() or err {
    result := process.shell("echo hello") or fail err;
    assert_eq(true, result.success);
    assert_eq(true, result.stdout.contains("hello"));
}

fn test_shell_with_pipe() or err {
    result := process.shell("echo 'one two three' | wc -w") or fail err;
    assert_eq(true, result.success);
    assert_eq(true, result.stdout.contains("3"));
}

fn test_shell_with_redirect() or err {
    result := process.shell("echo test > /tmp/bishop_test_output.txt && cat /tmp/bishop_test_output.txt") or fail err;
    assert_eq(true, result.success);
    assert_eq(true, result.stdout.contains("test"));
}

fn test_shell_command_substitution() or err {
    result := process.shell("echo $(echo inner)") or fail err;
    assert_eq(true, result.success);
    assert_eq(true, result.stdout.contains("inner"));
}

// ============================================================
// Tests for process.env and process.set_env
// ============================================================

fn test_env_read_existing() {
    home := process.env("HOME");
    assert_eq(true, home.length() > 0);
}

fn test_env_read_nonexistent() {
    value := process.env("NONEXISTENT_VAR_XYZ123");
    assert_eq(0, value.length());
}

fn test_set_env_and_read() {
    process.set_env("BISHOP_TEST_VAR", "test_value");
    value := process.env("BISHOP_TEST_VAR");
    assert_eq("test_value", value);
}

fn test_set_env_override() {
    process.set_env("BISHOP_TEST_VAR2", "original");
    process.set_env("BISHOP_TEST_VAR2", "updated");
    value := process.env("BISHOP_TEST_VAR2");
    assert_eq("updated", value);
}

fn test_env_with_default() {
    value := process.env("NONEXISTENT_VAR_ABC") default "default_value";
    assert_eq("default_value", value);
}

// ============================================================
// Tests for process.cwd and process.chdir
// ============================================================

fn test_cwd_returns_path() {
    current := process.cwd();
    assert_eq(true, current.length() > 0);
}

fn test_chdir_success() or err {
    original := process.cwd();
    process.chdir("/tmp") or fail err;
    new_dir := process.cwd();
    assert_eq("/tmp", new_dir);

    process.chdir(original) or fail err;
}

fn test_chdir_failure() or err {
    process.chdir("/nonexistent_directory_xyz123") or {
        return;
    };
    assert_eq(true, false);
}

// ============================================================
// Tests for process.args
// ============================================================

fn test_args_returns_list() {
    args := process.args();
    assert_eq(true, args.length() > 0);
}
