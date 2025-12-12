/**
 * @file http.hpp
 * @brief Nog HTTP runtime library.
 *
 * Provides async HTTP server functionality for Nog programs.
 * Uses ASIO for async I/O and llhttp for HTTP parsing.
 */

#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <functional>
#include <iostream>
#include <asio.hpp>
#include <asio/awaitable.hpp>
#include <llhttp.h>

namespace http {

/**
 * HTTP request structure.
 */
struct Request {
    std::string method;
    std::string path;
    std::string body;
};

/**
 * HTTP response structure.
 */
struct Response {
    int status;
    std::string content_type;
    std::string body;
};

/**
 * Creates a 200 OK text/plain response.
 */
Response text(const std::string& content);

/**
 * Creates a 200 OK application/json response.
 */
Response json(const std::string& content);

/**
 * Creates a 404 Not Found response.
 */
Response not_found();

/**
 * Formats an HTTP response for sending over the wire.
 */
std::string format_response(const Response& resp);

/**
 * Async request reader - reads until complete HTTP message is received.
 */
asio::awaitable<Request> read_request(asio::ip::tcp::socket& socket);

/**
 * Handle a single connection with an async handler.
 */
template<typename Handler>
asio::awaitable<void> handle_connection_async(asio::ip::tcp::socket socket, Handler handler) {
    try {
        Request req = co_await read_request(socket);
        Response resp = co_await handler(req);

        std::string response_str = format_response(resp);
        co_await asio::async_write(socket, asio::buffer(response_str), asio::use_awaitable);
    } catch (const std::exception& e) {
        // Connection closed or error
    }
}

/**
 * Main serve function - simple single-handler version.
 * Handler must be an async function returning asio::awaitable<Response>.
 */
template<typename Handler>
asio::awaitable<void> serve(int port, Handler handler) {
    auto executor = co_await asio::this_coro::executor;
    asio::ip::tcp::acceptor acceptor(executor, {asio::ip::tcp::v4(), static_cast<asio::ip::port_type>(port)});

    std::cout << "HTTP server listening on port " << port << std::endl;

    while (true) {
        asio::ip::tcp::socket socket = co_await acceptor.async_accept(asio::use_awaitable);
        asio::co_spawn(executor, handle_connection_async(std::move(socket), handler), asio::detached);
    }
}

/**
 * App struct for routing-based HTTP server.
 */
struct App {
    std::vector<std::tuple<std::string, std::string, std::function<asio::awaitable<Response>(Request)>>> routes;

    void get(const std::string& path, std::function<asio::awaitable<Response>(Request)> handler);
    void post(const std::string& path, std::function<asio::awaitable<Response>(Request)> handler);
    asio::awaitable<Response> route(const Request& req);
    asio::awaitable<void> listen(int port);
};

}  // namespace http
