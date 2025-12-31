/**
 * @file yaml.cpp
 * @brief YAML module AST definitions for Bishop.
 */

#include "yaml.hpp"
#include <fmt/format.h>

using namespace std;

namespace bishop::stdlib {

unique_ptr<Program> create_yaml_module() {
    auto program = make_unique<Program>();

    // Yaml :: struct {} (opaque type - implementation in C++)
    auto yaml_struct = make_unique<StructDef>();
    yaml_struct->name = "Yaml";
    yaml_struct->visibility = Visibility::Public;
    program->structs.push_back(move(yaml_struct));

    // fn parse(str input) -> yaml.Yaml  (fallible)
    auto parse_fn = make_unique<FunctionDef>();
    parse_fn->name = "parse";
    parse_fn->return_type = "yaml.Yaml";
    parse_fn->visibility = Visibility::Public;
    parse_fn->error_type = "err";  // Makes it fallible
    parse_fn->params.push_back({"str", "input"});
    program->functions.push_back(move(parse_fn));

    // fn stringify(yaml.Yaml value) -> str
    auto stringify_fn = make_unique<FunctionDef>();
    stringify_fn->name = "stringify";
    stringify_fn->return_type = "str";
    stringify_fn->visibility = Visibility::Public;
    stringify_fn->params.push_back({"yaml.Yaml", "value"});
    program->functions.push_back(move(stringify_fn));

    // fn object() -> yaml.Yaml
    auto object_fn = make_unique<FunctionDef>();
    object_fn->name = "object";
    object_fn->return_type = "yaml.Yaml";
    object_fn->visibility = Visibility::Public;
    program->functions.push_back(move(object_fn));

    // fn array() -> yaml.Yaml
    auto array_fn = make_unique<FunctionDef>();
    array_fn->name = "array";
    array_fn->return_type = "yaml.Yaml";
    array_fn->visibility = Visibility::Public;
    program->functions.push_back(move(array_fn));

    // Yaml :: is_object(self) -> bool
    auto is_object_method = make_unique<MethodDef>();
    is_object_method->struct_name = "Yaml";
    is_object_method->name = "is_object";
    is_object_method->visibility = Visibility::Public;
    is_object_method->params.push_back({"yaml.Yaml", "self"});
    is_object_method->return_type = "bool";
    program->methods.push_back(move(is_object_method));

    // Yaml :: is_list(self) -> bool
    auto is_list_method = make_unique<MethodDef>();
    is_list_method->struct_name = "Yaml";
    is_list_method->name = "is_list";
    is_list_method->visibility = Visibility::Public;
    is_list_method->params.push_back({"yaml.Yaml", "self"});
    is_list_method->return_type = "bool";
    program->methods.push_back(move(is_list_method));

    // Yaml :: is_str(self) -> bool
    auto is_str_method = make_unique<MethodDef>();
    is_str_method->struct_name = "Yaml";
    is_str_method->name = "is_str";
    is_str_method->visibility = Visibility::Public;
    is_str_method->params.push_back({"yaml.Yaml", "self"});
    is_str_method->return_type = "bool";
    program->methods.push_back(move(is_str_method));

    // Yaml :: is_int(self) -> bool
    auto is_int_method = make_unique<MethodDef>();
    is_int_method->struct_name = "Yaml";
    is_int_method->name = "is_int";
    is_int_method->visibility = Visibility::Public;
    is_int_method->params.push_back({"yaml.Yaml", "self"});
    is_int_method->return_type = "bool";
    program->methods.push_back(move(is_int_method));

    // Yaml :: is_float(self) -> bool
    auto is_float_method = make_unique<MethodDef>();
    is_float_method->struct_name = "Yaml";
    is_float_method->name = "is_float";
    is_float_method->visibility = Visibility::Public;
    is_float_method->params.push_back({"yaml.Yaml", "self"});
    is_float_method->return_type = "bool";
    program->methods.push_back(move(is_float_method));

    // Yaml :: is_bool(self) -> bool
    auto is_bool_method = make_unique<MethodDef>();
    is_bool_method->struct_name = "Yaml";
    is_bool_method->name = "is_bool";
    is_bool_method->visibility = Visibility::Public;
    is_bool_method->params.push_back({"yaml.Yaml", "self"});
    is_bool_method->return_type = "bool";
    program->methods.push_back(move(is_bool_method));

    // Yaml :: is_null(self) -> bool
    auto is_null_method = make_unique<MethodDef>();
    is_null_method->struct_name = "Yaml";
    is_null_method->name = "is_null";
    is_null_method->visibility = Visibility::Public;
    is_null_method->params.push_back({"yaml.Yaml", "self"});
    is_null_method->return_type = "bool";
    program->methods.push_back(move(is_null_method));

    // Yaml :: as_str(self) -> str (fallible)
    auto as_str_method = make_unique<MethodDef>();
    as_str_method->struct_name = "Yaml";
    as_str_method->name = "as_str";
    as_str_method->visibility = Visibility::Public;
    as_str_method->params.push_back({"yaml.Yaml", "self"});
    as_str_method->return_type = "str";
    as_str_method->error_type = "err";
    program->methods.push_back(move(as_str_method));

    // Yaml :: as_int(self) -> int (fallible)
    auto as_int_method = make_unique<MethodDef>();
    as_int_method->struct_name = "Yaml";
    as_int_method->name = "as_int";
    as_int_method->visibility = Visibility::Public;
    as_int_method->params.push_back({"yaml.Yaml", "self"});
    as_int_method->return_type = "int";
    as_int_method->error_type = "err";
    program->methods.push_back(move(as_int_method));

    // Yaml :: as_float(self) -> f64 (fallible)
    auto as_float_method = make_unique<MethodDef>();
    as_float_method->struct_name = "Yaml";
    as_float_method->name = "as_float";
    as_float_method->visibility = Visibility::Public;
    as_float_method->params.push_back({"yaml.Yaml", "self"});
    as_float_method->return_type = "f64";
    as_float_method->error_type = "err";
    program->methods.push_back(move(as_float_method));

    // Yaml :: as_bool(self) -> bool (fallible)
    auto as_bool_method = make_unique<MethodDef>();
    as_bool_method->struct_name = "Yaml";
    as_bool_method->name = "as_bool";
    as_bool_method->visibility = Visibility::Public;
    as_bool_method->params.push_back({"yaml.Yaml", "self"});
    as_bool_method->return_type = "bool";
    as_bool_method->error_type = "err";
    program->methods.push_back(move(as_bool_method));

    // Yaml :: get(self, str key) -> yaml.Yaml (fallible)
    auto get_method = make_unique<MethodDef>();
    get_method->struct_name = "Yaml";
    get_method->name = "get";
    get_method->visibility = Visibility::Public;
    get_method->params.push_back({"yaml.Yaml", "self"});
    get_method->params.push_back({"str", "key"});
    get_method->return_type = "yaml.Yaml";
    get_method->error_type = "err";
    program->methods.push_back(move(get_method));

    // Yaml :: get_str(self, str key) -> str (fallible)
    auto get_str_method = make_unique<MethodDef>();
    get_str_method->struct_name = "Yaml";
    get_str_method->name = "get_str";
    get_str_method->visibility = Visibility::Public;
    get_str_method->params.push_back({"yaml.Yaml", "self"});
    get_str_method->params.push_back({"str", "key"});
    get_str_method->return_type = "str";
    get_str_method->error_type = "err";
    program->methods.push_back(move(get_str_method));

    // Yaml :: get_int(self, str key) -> int (fallible)
    auto get_int_method = make_unique<MethodDef>();
    get_int_method->struct_name = "Yaml";
    get_int_method->name = "get_int";
    get_int_method->visibility = Visibility::Public;
    get_int_method->params.push_back({"yaml.Yaml", "self"});
    get_int_method->params.push_back({"str", "key"});
    get_int_method->return_type = "int";
    get_int_method->error_type = "err";
    program->methods.push_back(move(get_int_method));

    // Yaml :: get_bool(self, str key) -> bool (fallible)
    auto get_bool_method = make_unique<MethodDef>();
    get_bool_method->struct_name = "Yaml";
    get_bool_method->name = "get_bool";
    get_bool_method->visibility = Visibility::Public;
    get_bool_method->params.push_back({"yaml.Yaml", "self"});
    get_bool_method->params.push_back({"str", "key"});
    get_bool_method->return_type = "bool";
    get_bool_method->error_type = "err";
    program->methods.push_back(move(get_bool_method));

    // Yaml :: get_list(self, str key) -> yaml.Yaml (fallible)
    auto get_list_method = make_unique<MethodDef>();
    get_list_method->struct_name = "Yaml";
    get_list_method->name = "get_list";
    get_list_method->visibility = Visibility::Public;
    get_list_method->params.push_back({"yaml.Yaml", "self"});
    get_list_method->params.push_back({"str", "key"});
    get_list_method->return_type = "yaml.Yaml";
    get_list_method->error_type = "err";
    program->methods.push_back(move(get_list_method));

    // Yaml :: get_object(self, str key) -> yaml.Yaml (fallible)
    auto get_object_method = make_unique<MethodDef>();
    get_object_method->struct_name = "Yaml";
    get_object_method->name = "get_object";
    get_object_method->visibility = Visibility::Public;
    get_object_method->params.push_back({"yaml.Yaml", "self"});
    get_object_method->params.push_back({"str", "key"});
    get_object_method->return_type = "yaml.Yaml";
    get_object_method->error_type = "err";
    program->methods.push_back(move(get_object_method));

    // Yaml :: path(self, str path) -> yaml.Yaml (fallible)
    auto path_method = make_unique<MethodDef>();
    path_method->struct_name = "Yaml";
    path_method->name = "path";
    path_method->visibility = Visibility::Public;
    path_method->params.push_back({"yaml.Yaml", "self"});
    path_method->params.push_back({"str", "path"});
    path_method->return_type = "yaml.Yaml";
    path_method->error_type = "err";
    program->methods.push_back(move(path_method));

    // Yaml :: length(self) -> int
    auto length_method = make_unique<MethodDef>();
    length_method->struct_name = "Yaml";
    length_method->name = "length";
    length_method->visibility = Visibility::Public;
    length_method->params.push_back({"yaml.Yaml", "self"});
    length_method->return_type = "int";
    program->methods.push_back(move(length_method));

    // Yaml :: has(self, str key) -> bool
    auto has_method = make_unique<MethodDef>();
    has_method->struct_name = "Yaml";
    has_method->name = "has";
    has_method->visibility = Visibility::Public;
    has_method->params.push_back({"yaml.Yaml", "self"});
    has_method->params.push_back({"str", "key"});
    has_method->return_type = "bool";
    program->methods.push_back(move(has_method));

    // Yaml :: keys(self) -> List<str>
    auto keys_method = make_unique<MethodDef>();
    keys_method->struct_name = "Yaml";
    keys_method->name = "keys";
    keys_method->visibility = Visibility::Public;
    keys_method->params.push_back({"yaml.Yaml", "self"});
    keys_method->return_type = "List<str>";
    program->methods.push_back(move(keys_method));

    // Yaml :: set_str(self, str key, str value)
    auto set_str_method = make_unique<MethodDef>();
    set_str_method->struct_name = "Yaml";
    set_str_method->name = "set_str";
    set_str_method->visibility = Visibility::Public;
    set_str_method->params.push_back({"yaml.Yaml", "self"});
    set_str_method->params.push_back({"str", "key"});
    set_str_method->params.push_back({"str", "value"});
    program->methods.push_back(move(set_str_method));

    // Yaml :: set_int(self, str key, int value)
    auto set_int_method = make_unique<MethodDef>();
    set_int_method->struct_name = "Yaml";
    set_int_method->name = "set_int";
    set_int_method->visibility = Visibility::Public;
    set_int_method->params.push_back({"yaml.Yaml", "self"});
    set_int_method->params.push_back({"str", "key"});
    set_int_method->params.push_back({"int", "value"});
    program->methods.push_back(move(set_int_method));

    // Yaml :: set_bool(self, str key, bool value)
    auto set_bool_method = make_unique<MethodDef>();
    set_bool_method->struct_name = "Yaml";
    set_bool_method->name = "set_bool";
    set_bool_method->visibility = Visibility::Public;
    set_bool_method->params.push_back({"yaml.Yaml", "self"});
    set_bool_method->params.push_back({"str", "key"});
    set_bool_method->params.push_back({"bool", "value"});
    program->methods.push_back(move(set_bool_method));

    // Yaml :: set_float(self, str key, f64 value)
    auto set_float_method = make_unique<MethodDef>();
    set_float_method->struct_name = "Yaml";
    set_float_method->name = "set_float";
    set_float_method->visibility = Visibility::Public;
    set_float_method->params.push_back({"yaml.Yaml", "self"});
    set_float_method->params.push_back({"str", "key"});
    set_float_method->params.push_back({"f64", "value"});
    program->methods.push_back(move(set_float_method));

    // Yaml :: remove(self, str key)
    auto remove_method = make_unique<MethodDef>();
    remove_method->struct_name = "Yaml";
    remove_method->name = "remove";
    remove_method->visibility = Visibility::Public;
    remove_method->params.push_back({"yaml.Yaml", "self"});
    remove_method->params.push_back({"str", "key"});
    program->methods.push_back(move(remove_method));

    // Yaml :: push_int(self, int value)
    auto push_int_method = make_unique<MethodDef>();
    push_int_method->struct_name = "Yaml";
    push_int_method->name = "push_int";
    push_int_method->visibility = Visibility::Public;
    push_int_method->params.push_back({"yaml.Yaml", "self"});
    push_int_method->params.push_back({"int", "value"});
    program->methods.push_back(move(push_int_method));

    // Yaml :: push_str(self, str value)
    auto push_str_method = make_unique<MethodDef>();
    push_str_method->struct_name = "Yaml";
    push_str_method->name = "push_str";
    push_str_method->visibility = Visibility::Public;
    push_str_method->params.push_back({"yaml.Yaml", "self"});
    push_str_method->params.push_back({"str", "value"});
    program->methods.push_back(move(push_str_method));

    return program;
}

string generate_yaml_runtime() {
    return R"(
namespace yaml {

using Yaml = bishop::Yaml;

inline bishop::YamlResult<Yaml> parse(const std::string& input) {
    return bishop::YamlParser::parse(input);
}

inline std::string stringify(const Yaml& value) {
    return bishop::YamlSerializer::stringify(value);
}

inline Yaml object() {
    return Yaml::make_object();
}

inline Yaml array() {
    return Yaml::make_array();
}

}  // namespace yaml
)";
}

}  // namespace bishop::stdlib
