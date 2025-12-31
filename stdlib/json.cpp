/**
 * @file json.cpp
 * @brief Built-in json module implementation.
 *
 * Creates the AST definitions for the json module.
 * The actual runtime is in src/runtime/json/json.hpp and included as a header.
 */

/**
 * @nog_fn parse
 * @module json
 * @description Parses a JSON string into a Json value.
 * @param text str - The JSON string to parse
 * @returns Json - Parsed JSON value
 * @errors ParseError - If the JSON is malformed
 * @example
 * import json;
 * data := json.parse('{"name": "Alice", "age": 30}') or return;
 */

/**
 * @nog_fn object
 * @module json
 * @description Creates an empty JSON object.
 * @returns Json - An empty JSON object {}
 * @example
 * import json;
 * obj := json.object();
 * obj.set("name", "Bob");
 */

/**
 * @nog_fn array
 * @module json
 * @description Creates an empty JSON array.
 * @returns Json - An empty JSON array []
 * @example
 * import json;
 * arr := json.array();
 * arr.push(42);
 */

/**
 * @nog_fn stringify
 * @module json
 * @description Converts a Json value to a JSON string.
 * @param value Json - The JSON value to serialize
 * @returns str - JSON string representation
 * @example
 * import json;
 * text := json.stringify(data);
 */

/**
 * @nog_fn stringify_pretty
 * @module json
 * @description Converts a Json value to a pretty-printed JSON string.
 * @param value Json - The JSON value to serialize
 * @returns str - Pretty-printed JSON string
 * @example
 * import json;
 * text := json.stringify_pretty(data);
 */

/**
 * @nog_method get
 * @type Json
 * @description Gets a value by key (for objects) or index (for arrays).
 * @param key str - Key name or integer index as string
 * @returns Json - The value at the given key/index
 * @errors KeyError - If key does not exist
 * @example
 * name := data.get("name") or return;
 */

/**
 * @nog_method get_str
 * @type Json
 * @description Gets a string value by key.
 * @param key str - Key name
 * @returns str - The string value
 * @errors TypeError - If value is not a string
 * @errors KeyError - If key does not exist
 * @example
 * name := data.get_str("name") or return;
 */

/**
 * @nog_method get_int
 * @type Json
 * @description Gets an integer value by key.
 * @param key str - Key name
 * @returns int - The integer value
 * @errors TypeError - If value is not a number
 * @errors KeyError - If key does not exist
 * @example
 * age := data.get_int("age") or return;
 */

/**
 * @nog_method get_float
 * @type Json
 * @description Gets a float value by key.
 * @param key str - Key name
 * @returns float - The float value
 * @errors TypeError - If value is not a number
 * @errors KeyError - If key does not exist
 * @example
 * price := data.get_float("price") or return;
 */

/**
 * @nog_method get_bool
 * @type Json
 * @description Gets a boolean value by key.
 * @param key str - Key name
 * @returns bool - The boolean value
 * @errors TypeError - If value is not a boolean
 * @errors KeyError - If key does not exist
 * @example
 * active := data.get_bool("active") or return;
 */

/**
 * @nog_method get_list
 * @type Json
 * @description Gets an array value by key.
 * @param key str - Key name
 * @returns Json - The array value
 * @errors TypeError - If value is not an array
 * @errors KeyError - If key does not exist
 * @example
 * items := data.get_list("items") or return;
 */

/**
 * @nog_method get_object
 * @type Json
 * @description Gets an object value by key.
 * @param key str - Key name
 * @returns Json - The object value
 * @errors TypeError - If value is not an object
 * @errors KeyError - If key does not exist
 * @example
 * config := data.get_object("config") or return;
 */

/**
 * @nog_method path
 * @type Json
 * @description Accesses nested values using dot notation path.
 * @param path str - Dot-separated path (e.g., "user.address.city")
 * @returns Json - The value at the path
 * @errors KeyError - If path does not exist
 * @example
 * city := data.path("user.address.city") or return;
 */

/**
 * @nog_method is_null
 * @type Json
 * @description Checks if the JSON value is null.
 * @returns bool - True if null
 * @example
 * if data.is_null() { print("Value is null"); }
 */

/**
 * @nog_method is_str
 * @type Json
 * @description Checks if the JSON value is a string.
 * @returns bool - True if string
 * @example
 * if data.is_str() { name := data.as_str(); }
 */

/**
 * @nog_method is_int
 * @type Json
 * @description Checks if the JSON value is an integer.
 * @returns bool - True if integer
 * @example
 * if data.is_int() { count := data.as_int(); }
 */

/**
 * @nog_method is_float
 * @type Json
 * @description Checks if the JSON value is a float.
 * @returns bool - True if float
 * @example
 * if data.is_float() { value := data.as_float(); }
 */

/**
 * @nog_method is_bool
 * @type Json
 * @description Checks if the JSON value is a boolean.
 * @returns bool - True if boolean
 * @example
 * if data.is_bool() { flag := data.as_bool(); }
 */

/**
 * @nog_method is_list
 * @type Json
 * @description Checks if the JSON value is an array.
 * @returns bool - True if array
 * @example
 * if data.is_list() { items := data.as_list(); }
 */

/**
 * @nog_method is_object
 * @type Json
 * @description Checks if the JSON value is an object.
 * @returns bool - True if object
 * @example
 * if data.is_object() { keys := data.keys(); }
 */

/**
 * @nog_method as_str
 * @type Json
 * @description Converts the JSON value to a string.
 * @returns str - The string value
 * @errors TypeError - If value is not a string
 * @example
 * name := data.as_str() or return;
 */

/**
 * @nog_method as_int
 * @type Json
 * @description Converts the JSON value to an integer.
 * @returns int - The integer value
 * @errors TypeError - If value is not a number
 * @example
 * count := data.as_int() or return;
 */

/**
 * @nog_method as_float
 * @type Json
 * @description Converts the JSON value to a float.
 * @returns float - The float value
 * @errors TypeError - If value is not a number
 * @example
 * price := data.as_float() or return;
 */

/**
 * @nog_method as_bool
 * @type Json
 * @description Converts the JSON value to a boolean.
 * @returns bool - The boolean value
 * @errors TypeError - If value is not a boolean
 * @example
 * active := data.as_bool() or return;
 */

/**
 * @nog_method set
 * @type Json
 * @description Sets a value in a JSON object.
 * @param key str - The key name
 * @param value Json - The value to set
 * @example
 * obj.set("name", json.parse('"Alice"'));
 */

/**
 * @nog_method set_str
 * @type Json
 * @description Sets a string value in a JSON object.
 * @param key str - The key name
 * @param value str - The string value
 * @example
 * obj.set_str("name", "Alice");
 */

/**
 * @nog_method set_int
 * @type Json
 * @description Sets an integer value in a JSON object.
 * @param key str - The key name
 * @param value int - The integer value
 * @example
 * obj.set_int("age", 30);
 */

/**
 * @nog_method set_float
 * @type Json
 * @description Sets a float value in a JSON object.
 * @param key str - The key name
 * @param value float - The float value
 * @example
 * obj.set_float("price", 19.99);
 */

/**
 * @nog_method set_bool
 * @type Json
 * @description Sets a boolean value in a JSON object.
 * @param key str - The key name
 * @param value bool - The boolean value
 * @example
 * obj.set_bool("active", true);
 */

/**
 * @nog_method set_null
 * @type Json
 * @description Sets a null value in a JSON object.
 * @param key str - The key name
 * @example
 * obj.set_null("optional_field");
 */

/**
 * @nog_method push
 * @type Json
 * @description Appends a value to a JSON array.
 * @param value Json - The value to append
 * @example
 * arr.push(json.parse("42"));
 */

/**
 * @nog_method push_str
 * @type Json
 * @description Appends a string to a JSON array.
 * @param value str - The string to append
 * @example
 * arr.push_str("hello");
 */

/**
 * @nog_method push_int
 * @type Json
 * @description Appends an integer to a JSON array.
 * @param value int - The integer to append
 * @example
 * arr.push_int(42);
 */

/**
 * @nog_method push_float
 * @type Json
 * @description Appends a float to a JSON array.
 * @param value float - The float to append
 * @example
 * arr.push_float(3.14);
 */

/**
 * @nog_method push_bool
 * @type Json
 * @description Appends a boolean to a JSON array.
 * @param value bool - The boolean to append
 * @example
 * arr.push_bool(true);
 */

/**
 * @nog_method push_null
 * @type Json
 * @description Appends a null value to a JSON array.
 * @example
 * arr.push_null();
 */

/**
 * @nog_method length
 * @type Json
 * @description Returns the length of a JSON array or object.
 * @returns int - Number of elements/keys
 * @example
 * count := arr.length();
 */

/**
 * @nog_method keys
 * @type Json
 * @description Returns the keys of a JSON object as newline-separated string.
 * @returns str - Newline-separated list of keys
 * @example
 * key_list := obj.keys();
 */

/**
 * @nog_method has
 * @type Json
 * @description Checks if a JSON object has a key.
 * @param key str - The key to check
 * @returns bool - True if key exists
 * @example
 * if obj.has("name") { ... }
 */

/**
 * @nog_method remove
 * @type Json
 * @description Removes a key from a JSON object.
 * @param key str - The key to remove
 * @example
 * obj.remove("temp_field");
 */

#include "json.hpp"

using namespace std;

namespace bishop::stdlib {

/**
 * Creates the AST for the built-in json module.
 */
unique_ptr<Program> create_json_module() {
    auto program = make_unique<Program>();

    // Json :: struct { } (internal state managed in C++)
    auto json_struct = make_unique<StructDef>();
    json_struct->name = "Json";
    json_struct->visibility = Visibility::Public;
    program->structs.push_back(move(json_struct));

    // fn parse(str text) -> json.Json or err
    auto parse_fn = make_unique<FunctionDef>();
    parse_fn->name = "parse";
    parse_fn->visibility = Visibility::Public;
    parse_fn->params.push_back({"str", "text"});
    parse_fn->return_type = "json.Json";
    parse_fn->error_type = "err";
    program->functions.push_back(move(parse_fn));

    // fn object() -> json.Json
    auto object_fn = make_unique<FunctionDef>();
    object_fn->name = "object";
    object_fn->visibility = Visibility::Public;
    object_fn->return_type = "json.Json";
    program->functions.push_back(move(object_fn));

    // fn array() -> json.Json
    auto array_fn = make_unique<FunctionDef>();
    array_fn->name = "array";
    array_fn->visibility = Visibility::Public;
    array_fn->return_type = "json.Json";
    program->functions.push_back(move(array_fn));

    // fn stringify(json.Json value) -> str
    auto stringify_fn = make_unique<FunctionDef>();
    stringify_fn->name = "stringify";
    stringify_fn->visibility = Visibility::Public;
    stringify_fn->params.push_back({"json.Json", "value"});
    stringify_fn->return_type = "str";
    program->functions.push_back(move(stringify_fn));

    // fn stringify_pretty(json.Json value) -> str
    auto stringify_pretty_fn = make_unique<FunctionDef>();
    stringify_pretty_fn->name = "stringify_pretty";
    stringify_pretty_fn->visibility = Visibility::Public;
    stringify_pretty_fn->params.push_back({"json.Json", "value"});
    stringify_pretty_fn->return_type = "str";
    program->functions.push_back(move(stringify_pretty_fn));

    // ===== Json Methods =====

    // Json :: is_null(self) -> bool
    auto is_null_method = make_unique<MethodDef>();
    is_null_method->struct_name = "Json";
    is_null_method->name = "is_null";
    is_null_method->visibility = Visibility::Public;
    is_null_method->params.push_back({"json.Json", "self"});
    is_null_method->return_type = "bool";
    program->methods.push_back(move(is_null_method));

    // Json :: is_str(self) -> bool
    auto is_str_method = make_unique<MethodDef>();
    is_str_method->struct_name = "Json";
    is_str_method->name = "is_str";
    is_str_method->visibility = Visibility::Public;
    is_str_method->params.push_back({"json.Json", "self"});
    is_str_method->return_type = "bool";
    program->methods.push_back(move(is_str_method));

    // Json :: is_int(self) -> bool
    auto is_int_method = make_unique<MethodDef>();
    is_int_method->struct_name = "Json";
    is_int_method->name = "is_int";
    is_int_method->visibility = Visibility::Public;
    is_int_method->params.push_back({"json.Json", "self"});
    is_int_method->return_type = "bool";
    program->methods.push_back(move(is_int_method));

    // Json :: is_float(self) -> bool
    auto is_float_method = make_unique<MethodDef>();
    is_float_method->struct_name = "Json";
    is_float_method->name = "is_float";
    is_float_method->visibility = Visibility::Public;
    is_float_method->params.push_back({"json.Json", "self"});
    is_float_method->return_type = "bool";
    program->methods.push_back(move(is_float_method));

    // Json :: is_bool(self) -> bool
    auto is_bool_method = make_unique<MethodDef>();
    is_bool_method->struct_name = "Json";
    is_bool_method->name = "is_bool";
    is_bool_method->visibility = Visibility::Public;
    is_bool_method->params.push_back({"json.Json", "self"});
    is_bool_method->return_type = "bool";
    program->methods.push_back(move(is_bool_method));

    // Json :: is_list(self) -> bool
    auto is_list_method = make_unique<MethodDef>();
    is_list_method->struct_name = "Json";
    is_list_method->name = "is_list";
    is_list_method->visibility = Visibility::Public;
    is_list_method->params.push_back({"json.Json", "self"});
    is_list_method->return_type = "bool";
    program->methods.push_back(move(is_list_method));

    // Json :: is_object(self) -> bool
    auto is_object_method = make_unique<MethodDef>();
    is_object_method->struct_name = "Json";
    is_object_method->name = "is_object";
    is_object_method->visibility = Visibility::Public;
    is_object_method->params.push_back({"json.Json", "self"});
    is_object_method->return_type = "bool";
    program->methods.push_back(move(is_object_method));

    // Json :: as_str(self) -> str or err
    auto as_str_method = make_unique<MethodDef>();
    as_str_method->struct_name = "Json";
    as_str_method->name = "as_str";
    as_str_method->visibility = Visibility::Public;
    as_str_method->params.push_back({"json.Json", "self"});
    as_str_method->return_type = "str";
    as_str_method->error_type = "err";
    program->methods.push_back(move(as_str_method));

    // Json :: as_int(self) -> int or err
    auto as_int_method = make_unique<MethodDef>();
    as_int_method->struct_name = "Json";
    as_int_method->name = "as_int";
    as_int_method->visibility = Visibility::Public;
    as_int_method->params.push_back({"json.Json", "self"});
    as_int_method->return_type = "int";
    as_int_method->error_type = "err";
    program->methods.push_back(move(as_int_method));

    // Json :: as_float(self) -> f64 or err
    auto as_float_method = make_unique<MethodDef>();
    as_float_method->struct_name = "Json";
    as_float_method->name = "as_float";
    as_float_method->visibility = Visibility::Public;
    as_float_method->params.push_back({"json.Json", "self"});
    as_float_method->return_type = "f64";
    as_float_method->error_type = "err";
    program->methods.push_back(move(as_float_method));

    // Json :: as_bool(self) -> bool or err
    auto as_bool_method = make_unique<MethodDef>();
    as_bool_method->struct_name = "Json";
    as_bool_method->name = "as_bool";
    as_bool_method->visibility = Visibility::Public;
    as_bool_method->params.push_back({"json.Json", "self"});
    as_bool_method->return_type = "bool";
    as_bool_method->error_type = "err";
    program->methods.push_back(move(as_bool_method));

    // Json :: get(self, str key) -> json.Json or err
    auto get_method = make_unique<MethodDef>();
    get_method->struct_name = "Json";
    get_method->name = "get";
    get_method->visibility = Visibility::Public;
    get_method->params.push_back({"json.Json", "self"});
    get_method->params.push_back({"str", "key"});
    get_method->return_type = "json.Json";
    get_method->error_type = "err";
    program->methods.push_back(move(get_method));

    // Json :: get_str(self, str key) -> str or err
    auto get_str_method = make_unique<MethodDef>();
    get_str_method->struct_name = "Json";
    get_str_method->name = "get_str";
    get_str_method->visibility = Visibility::Public;
    get_str_method->params.push_back({"json.Json", "self"});
    get_str_method->params.push_back({"str", "key"});
    get_str_method->return_type = "str";
    get_str_method->error_type = "err";
    program->methods.push_back(move(get_str_method));

    // Json :: get_int(self, str key) -> int or err
    auto get_int_method = make_unique<MethodDef>();
    get_int_method->struct_name = "Json";
    get_int_method->name = "get_int";
    get_int_method->visibility = Visibility::Public;
    get_int_method->params.push_back({"json.Json", "self"});
    get_int_method->params.push_back({"str", "key"});
    get_int_method->return_type = "int";
    get_int_method->error_type = "err";
    program->methods.push_back(move(get_int_method));

    // Json :: get_float(self, str key) -> f64 or err
    auto get_float_method = make_unique<MethodDef>();
    get_float_method->struct_name = "Json";
    get_float_method->name = "get_float";
    get_float_method->visibility = Visibility::Public;
    get_float_method->params.push_back({"json.Json", "self"});
    get_float_method->params.push_back({"str", "key"});
    get_float_method->return_type = "f64";
    get_float_method->error_type = "err";
    program->methods.push_back(move(get_float_method));

    // Json :: get_bool(self, str key) -> bool or err
    auto get_bool_method = make_unique<MethodDef>();
    get_bool_method->struct_name = "Json";
    get_bool_method->name = "get_bool";
    get_bool_method->visibility = Visibility::Public;
    get_bool_method->params.push_back({"json.Json", "self"});
    get_bool_method->params.push_back({"str", "key"});
    get_bool_method->return_type = "bool";
    get_bool_method->error_type = "err";
    program->methods.push_back(move(get_bool_method));

    // Json :: get_list(self, str key) -> json.Json or err
    auto get_list_method = make_unique<MethodDef>();
    get_list_method->struct_name = "Json";
    get_list_method->name = "get_list";
    get_list_method->visibility = Visibility::Public;
    get_list_method->params.push_back({"json.Json", "self"});
    get_list_method->params.push_back({"str", "key"});
    get_list_method->return_type = "json.Json";
    get_list_method->error_type = "err";
    program->methods.push_back(move(get_list_method));

    // Json :: get_object(self, str key) -> json.Json or err
    auto get_object_method = make_unique<MethodDef>();
    get_object_method->struct_name = "Json";
    get_object_method->name = "get_object";
    get_object_method->visibility = Visibility::Public;
    get_object_method->params.push_back({"json.Json", "self"});
    get_object_method->params.push_back({"str", "key"});
    get_object_method->return_type = "json.Json";
    get_object_method->error_type = "err";
    program->methods.push_back(move(get_object_method));

    // Json :: path(self, str path) -> json.Json or err
    auto path_method = make_unique<MethodDef>();
    path_method->struct_name = "Json";
    path_method->name = "path";
    path_method->visibility = Visibility::Public;
    path_method->params.push_back({"json.Json", "self"});
    path_method->params.push_back({"str", "path"});
    path_method->return_type = "json.Json";
    path_method->error_type = "err";
    program->methods.push_back(move(path_method));

    // Json :: set(self, str key, json.Json value)
    auto set_method = make_unique<MethodDef>();
    set_method->struct_name = "Json";
    set_method->name = "set";
    set_method->visibility = Visibility::Public;
    set_method->params.push_back({"json.Json", "self"});
    set_method->params.push_back({"str", "key"});
    set_method->params.push_back({"json.Json", "value"});
    program->methods.push_back(move(set_method));

    // Json :: set_str(self, str key, str value)
    auto set_str_method = make_unique<MethodDef>();
    set_str_method->struct_name = "Json";
    set_str_method->name = "set_str";
    set_str_method->visibility = Visibility::Public;
    set_str_method->params.push_back({"json.Json", "self"});
    set_str_method->params.push_back({"str", "key"});
    set_str_method->params.push_back({"str", "value"});
    program->methods.push_back(move(set_str_method));

    // Json :: set_int(self, str key, int value)
    auto set_int_method = make_unique<MethodDef>();
    set_int_method->struct_name = "Json";
    set_int_method->name = "set_int";
    set_int_method->visibility = Visibility::Public;
    set_int_method->params.push_back({"json.Json", "self"});
    set_int_method->params.push_back({"str", "key"});
    set_int_method->params.push_back({"int", "value"});
    program->methods.push_back(move(set_int_method));

    // Json :: set_float(self, str key, f64 value)
    auto set_float_method = make_unique<MethodDef>();
    set_float_method->struct_name = "Json";
    set_float_method->name = "set_float";
    set_float_method->visibility = Visibility::Public;
    set_float_method->params.push_back({"json.Json", "self"});
    set_float_method->params.push_back({"str", "key"});
    set_float_method->params.push_back({"f64", "value"});
    program->methods.push_back(move(set_float_method));

    // Json :: set_bool(self, str key, bool value)
    auto set_bool_method = make_unique<MethodDef>();
    set_bool_method->struct_name = "Json";
    set_bool_method->name = "set_bool";
    set_bool_method->visibility = Visibility::Public;
    set_bool_method->params.push_back({"json.Json", "self"});
    set_bool_method->params.push_back({"str", "key"});
    set_bool_method->params.push_back({"bool", "value"});
    program->methods.push_back(move(set_bool_method));

    // Json :: set_null(self, str key)
    auto set_null_method = make_unique<MethodDef>();
    set_null_method->struct_name = "Json";
    set_null_method->name = "set_null";
    set_null_method->visibility = Visibility::Public;
    set_null_method->params.push_back({"json.Json", "self"});
    set_null_method->params.push_back({"str", "key"});
    program->methods.push_back(move(set_null_method));

    // Json :: push(self, json.Json value)
    auto push_method = make_unique<MethodDef>();
    push_method->struct_name = "Json";
    push_method->name = "push";
    push_method->visibility = Visibility::Public;
    push_method->params.push_back({"json.Json", "self"});
    push_method->params.push_back({"json.Json", "value"});
    program->methods.push_back(move(push_method));

    // Json :: push_str(self, str value)
    auto push_str_method = make_unique<MethodDef>();
    push_str_method->struct_name = "Json";
    push_str_method->name = "push_str";
    push_str_method->visibility = Visibility::Public;
    push_str_method->params.push_back({"json.Json", "self"});
    push_str_method->params.push_back({"str", "value"});
    program->methods.push_back(move(push_str_method));

    // Json :: push_int(self, int value)
    auto push_int_method = make_unique<MethodDef>();
    push_int_method->struct_name = "Json";
    push_int_method->name = "push_int";
    push_int_method->visibility = Visibility::Public;
    push_int_method->params.push_back({"json.Json", "self"});
    push_int_method->params.push_back({"int", "value"});
    program->methods.push_back(move(push_int_method));

    // Json :: push_float(self, f64 value)
    auto push_float_method = make_unique<MethodDef>();
    push_float_method->struct_name = "Json";
    push_float_method->name = "push_float";
    push_float_method->visibility = Visibility::Public;
    push_float_method->params.push_back({"json.Json", "self"});
    push_float_method->params.push_back({"f64", "value"});
    program->methods.push_back(move(push_float_method));

    // Json :: push_bool(self, bool value)
    auto push_bool_method = make_unique<MethodDef>();
    push_bool_method->struct_name = "Json";
    push_bool_method->name = "push_bool";
    push_bool_method->visibility = Visibility::Public;
    push_bool_method->params.push_back({"json.Json", "self"});
    push_bool_method->params.push_back({"bool", "value"});
    program->methods.push_back(move(push_bool_method));

    // Json :: push_null(self)
    auto push_null_method = make_unique<MethodDef>();
    push_null_method->struct_name = "Json";
    push_null_method->name = "push_null";
    push_null_method->visibility = Visibility::Public;
    push_null_method->params.push_back({"json.Json", "self"});
    program->methods.push_back(move(push_null_method));

    // Json :: length(self) -> int
    auto length_method = make_unique<MethodDef>();
    length_method->struct_name = "Json";
    length_method->name = "length";
    length_method->visibility = Visibility::Public;
    length_method->params.push_back({"json.Json", "self"});
    length_method->return_type = "int";
    program->methods.push_back(move(length_method));

    // Json :: keys(self) -> str
    auto keys_method = make_unique<MethodDef>();
    keys_method->struct_name = "Json";
    keys_method->name = "keys";
    keys_method->visibility = Visibility::Public;
    keys_method->params.push_back({"json.Json", "self"});
    keys_method->return_type = "str";
    program->methods.push_back(move(keys_method));

    // Json :: has(self, str key) -> bool
    auto has_method = make_unique<MethodDef>();
    has_method->struct_name = "Json";
    has_method->name = "has";
    has_method->visibility = Visibility::Public;
    has_method->params.push_back({"json.Json", "self"});
    has_method->params.push_back({"str", "key"});
    has_method->return_type = "bool";
    program->methods.push_back(move(has_method));

    // Json :: remove(self, str key)
    auto remove_method = make_unique<MethodDef>();
    remove_method->struct_name = "Json";
    remove_method->name = "remove";
    remove_method->visibility = Visibility::Public;
    remove_method->params.push_back({"json.Json", "self"});
    remove_method->params.push_back({"str", "key"});
    program->methods.push_back(move(remove_method));

    return program;
}

/**
 * Returns empty - json.hpp is included at the top of generated code
 * for precompiled header support.
 */
string generate_json_runtime() {
    return "";
}

}  // namespace bishop::stdlib
