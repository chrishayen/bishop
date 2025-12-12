/**
 * @file http.cpp
 * @brief Built-in HTTP module implementation.
 *
 * Creates the AST definitions for the http module.
 * The actual runtime is in src/runtime/http/http.hpp and linked as a library.
 */

#include "http.hpp"

using namespace std;

namespace nog::stdlib {

/**
 * Checks if a module name is a built-in stdlib module.
 */
bool is_builtin_module(const string& name) {
    return name == "http";
}

/**
 * Creates the AST for the built-in http module.
 */
unique_ptr<Program> create_http_module() {
    auto program = make_unique<Program>();

    // Request :: struct { method str, path str, body str }
    auto request = make_unique<StructDef>();
    request->name = "Request";
    request->visibility = Visibility::Public;
    request->fields.push_back({"method", "str", ""});
    request->fields.push_back({"path", "str", ""});
    request->fields.push_back({"body", "str", ""});
    program->structs.push_back(move(request));

    // Response :: struct { status int, content_type str, body str }
    auto response = make_unique<StructDef>();
    response->name = "Response";
    response->visibility = Visibility::Public;
    response->fields.push_back({"status", "int", ""});
    response->fields.push_back({"content_type", "str", ""});
    response->fields.push_back({"body", "str", ""});
    program->structs.push_back(move(response));

    // fn text(str content) -> http.Response
    auto text_fn = make_unique<FunctionDef>();
    text_fn->name = "text";
    text_fn->visibility = Visibility::Public;
    text_fn->params.push_back({"str", "content"});
    text_fn->return_type = "http.Response";
    program->functions.push_back(move(text_fn));

    // fn json(str content) -> http.Response
    auto json_fn = make_unique<FunctionDef>();
    json_fn->name = "json";
    json_fn->visibility = Visibility::Public;
    json_fn->params.push_back({"str", "content"});
    json_fn->return_type = "http.Response";
    program->functions.push_back(move(json_fn));

    // fn not_found() -> http.Response
    auto not_found_fn = make_unique<FunctionDef>();
    not_found_fn->name = "not_found";
    not_found_fn->visibility = Visibility::Public;
    not_found_fn->return_type = "http.Response";
    program->functions.push_back(move(not_found_fn));

    // async fn serve(int port, fn(http.Request) -> http.Response handler)
    auto serve_fn = make_unique<FunctionDef>();
    serve_fn->name = "serve";
    serve_fn->visibility = Visibility::Public;
    serve_fn->is_async = true;
    serve_fn->params.push_back({"int", "port"});
    serve_fn->params.push_back({"fn(http.Request) -> http.Response", "handler"});
    program->functions.push_back(move(serve_fn));

    // App :: struct { } (internal routing state managed in C++)
    auto app_struct = make_unique<StructDef>();
    app_struct->name = "App";
    app_struct->visibility = Visibility::Public;
    program->structs.push_back(move(app_struct));

    // App :: get(self, str path, fn(http.Request) -> http.Response handler)
    auto get_method = make_unique<MethodDef>();
    get_method->struct_name = "App";
    get_method->name = "get";
    get_method->visibility = Visibility::Public;
    get_method->params.push_back({"http.App", "self"});
    get_method->params.push_back({"str", "path"});
    get_method->params.push_back({"fn(http.Request) -> http.Response", "handler"});
    program->methods.push_back(move(get_method));

    // App :: post(self, str path, fn(http.Request) -> http.Response handler)
    auto post_method = make_unique<MethodDef>();
    post_method->struct_name = "App";
    post_method->name = "post";
    post_method->visibility = Visibility::Public;
    post_method->params.push_back({"http.App", "self"});
    post_method->params.push_back({"str", "path"});
    post_method->params.push_back({"fn(http.Request) -> http.Response", "handler"});
    program->methods.push_back(move(post_method));

    // App :: listen(self, int port) async
    auto listen_method = make_unique<MethodDef>();
    listen_method->struct_name = "App";
    listen_method->name = "listen";
    listen_method->visibility = Visibility::Public;
    listen_method->is_async = true;
    listen_method->params.push_back({"http.App", "self"});
    listen_method->params.push_back({"int", "port"});
    program->methods.push_back(move(listen_method));

    return program;
}

/**
 * Returns the include directive for the http runtime library.
 */
string generate_http_runtime() {
    return "#include <nog/http.hpp>\n";
}

}  // namespace nog::stdlib
