/// Tests for the process module

import process;

// ============================================================
// Tests for process.run
// ============================================================

fn test_run_simple_command() or err {
    result := process.run("echo", ["hello"]) or fail err;
    assert_eq(true, result.success);
    assert_eq(0, result.exit_code);
    assert_eq(true, result.output.contains("hello"));
}

fn test_run_ls_command() or err {
    result := process.run("ls", ["-la"]) or fail err;
    assert_eq(true, result.success);
    assert_eq(0, result.exit_code);
    assert_eq(true, result.output.length() > 0);
}

fn test_run_with_multiple_args() or err {
    result := process.run("echo", ["one", "two", "three"]) or fail err;
    assert_eq(true, result.success);
    assert_eq(true, result.output.contains("one"));
    assert_eq(true, result.output.contains("two"));
    assert_eq(true, result.output.contains("three"));
}

fn test_run_command_not_found() or err {
    result := process.run("nonexistent_command_xyz123", List<str>()) or {
        return;
    };
    assert_eq(false, result.success);
    assert_eq(true, result.exit_code != 0);
}

fn test_run_command_exit_code() or err {
    result := process.run("false", List<str>()) or fail err;
    assert_eq(false, result.success);
    assert_eq(1, result.exit_code);
}

// ============================================================
// Tests for process.shell
// ============================================================

fn test_shell_simple() or err {
    result := process.shell("echo hello") or fail err;
    assert_eq(true, result.success);
    assert_eq(true, result.output.contains("hello"));
}

fn test_shell_with_pipe() or err {
    result := process.shell("echo 'one two three' | wc -w") or fail err;
    assert_eq(true, result.success);
    assert_eq(true, result.output.contains("3"));
}

fn test_shell_with_redirect() or err {
    result := process.shell("echo test > /tmp/bishop_test_output.txt && cat /tmp/bishop_test_output.txt") or fail err;
    assert_eq(true, result.success);
    assert_eq(true, result.output.contains("test"));
}

fn test_shell_command_substitution() or err {
    result := process.shell("echo $(echo inner)") or fail err;
    assert_eq(true, result.success);
    assert_eq(true, result.output.contains("inner"));
}

// ============================================================
// Tests for process.env and process.set_env
// ============================================================

fn test_env_read_existing() or err {
    home := process.env("HOME") or fail err;
    assert_eq(true, home.length() > 0);
}

fn test_env_read_nonexistent_fails() or err {
    value := process.env("NONEXISTENT_VAR_XYZ123") or {
        return;
    };
    assert_eq(true, false);
}

fn test_set_env_and_read() or err {
    _ := process.set_env("BISHOP_TEST_VAR", "test_value") or fail err;
    value := process.env("BISHOP_TEST_VAR") or fail err;
    assert_eq("test_value", value);
}

fn test_set_env_override() or err {
    _a := process.set_env("BISHOP_TEST_VAR2", "original") or fail err;
    _b := process.set_env("BISHOP_TEST_VAR2", "updated") or fail err;
    value := process.env("BISHOP_TEST_VAR2") or fail err;
    assert_eq("updated", value);
}

fn test_env_with_or_handler() or err {
    value := process.env("NONEXISTENT_VAR_ABC") or {
        return;
    };

    assert_eq(true, false);
}

fn test_set_env_empty_name_fails() or err {
    _ := process.set_env("", "value") or {
        return;
    };
    assert_eq(true, false);
}

// ============================================================
// Tests for process.cwd and process.chdir
// ============================================================

fn test_cwd_returns_path() or err {
    current := process.cwd() or fail err;
    assert_eq(true, current.length() > 0);
}

fn test_chdir_success() or err {
    original := process.cwd() or fail err;
    _a := process.chdir("/tmp") or fail err;
    new_dir := process.cwd() or fail err;
    assert_eq("/tmp", new_dir);

    _b := process.chdir(original) or fail err;
}

fn test_chdir_failure() or err {
    _ := process.chdir("/nonexistent_directory_xyz123") or {
        return;
    };
    assert_eq(true, false);
}

fn test_cwd_with_or_handler() or err {
    dir := process.cwd() or fail err;
    assert_eq(true, dir.length() > 0);
}

// ============================================================
// Tests for process.args
// ============================================================

fn test_args_returns_list() {
    args := process.args();
    assert_eq(true, args.length() >= 0);
}
