/// Test the user's original example syntax

NotFound :: err;
EmptyFile :: err;

// Define the struct for static methods
SkillFile :: struct {}

@static
SkillFile :: read_content(str path) -> str or err {
    // Using bare error type in or fail
    path.length() > 0 or fail NotFound;

    // Using braceless if/else with fail
    content := "sample content";

    if content.empty() fail EmptyFile;
    else return content;
}

fn test_read_content_success() or err {
    result := SkillFile.read_content("/some/path") or fail err;
    assert_eq(result, "sample content");
}

fn test_read_content_empty_path() {
    result := SkillFile.read_content("") or match err {
        NotFound => "not found",
        _ => "other"
    };
    assert_eq(result, "not found");
}
