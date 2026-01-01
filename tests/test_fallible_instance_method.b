/// Test fallible instance methods (methods with self that return or err)

NotFound :: err;
ParseError :: err;

// A struct with fallible instance methods
Parser :: struct
{
    data str
}

// Fallible instance method returning a value
Parser :: parse(self) -> int or err
{
    if self.data.empty()
    {
        fail NotFound;
    }
    return self.data.to_int() or fail ParseError;
}

// Fallible instance method returning void
Parser :: validate(self) or err
{
    if self.data.empty()
    {
        fail NotFound;
    }
}

// Instance method that propagates errors with or fail err
Parser :: parse_or_default(self, int fallback) -> int or err
{
    result := self.parse() or fail err;
    return result;
}

fn test_fallible_instance_method_success() or err
{
    p := Parser { data: "42" };
    result := p.parse() or fail err;
    assert_eq(result, 42);
}

fn test_fallible_instance_method_failure()
{
    p := Parser { data: "" };
    result := p.parse() or match err {
        NotFound => 999,
        _ => 0
    };
    assert_eq(result, 999);
}

fn test_fallible_instance_method_void_success() or err
{
    p := Parser { data: "hello" };
    p.validate() or fail err;
    assert_true(true);
}

fn test_fallible_instance_method_void_failure()
{
    p := Parser { data: "" };
    failed := false;
    p.validate() or {
        failed = true;
    };
    assert_true(failed);
}

fn test_fallible_instance_method_propagation() or err
{
    p := Parser { data: "123" };
    result := p.parse_or_default(0) or fail err;
    assert_eq(result, 123);
}

fn test_fallible_instance_method_propagation_failure()
{
    p := Parser { data: "" };
    result := p.parse_or_default(0) or match err {
        NotFound => 888,
        _ => 0
    };
    assert_eq(result, 888);
}
