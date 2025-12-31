/**
 * @file net.cpp
 * @brief Built-in net (networking) module implementation.
 *
 * Creates the AST definitions for the net module.
 * The actual runtime is in runtime/net/net.hpp and included as a header.
 */

/**
 * @bishop_struct TcpListener
 * @module net
 * @description A TCP server socket that listens for incoming connections.
 * Supports the `with` statement for automatic resource cleanup.
 * @example
 * server := net.listen("127.0.0.1", 8080) or return;
 * with server {
 *     while true {
 *         conn := server.accept() or continue;
 *         // handle connection
 *     }
 * }
 */

/**
 * @bishop_struct TcpStream
 * @module net
 * @description A TCP connection stream for reading and writing data.
 * Supports the `with` statement for automatic resource cleanup.
 * @example
 * conn := net.connect("example.com", 80) or return;
 * conn.write("GET / HTTP/1.1\r\n\r\n");
 * response := conn.read(4096);
 * conn.close();
 */

/**
 * @bishop_struct UdpSocket
 * @module net
 * @description A UDP socket for sending and receiving datagrams.
 * Supports the `with` statement for automatic resource cleanup.
 * @example
 * sock := net.udp_bind("0.0.0.0", 9000) or return;
 * sock.send_to("hello", "192.168.1.1", 9000);
 * sock.close();
 */

/**
 * @bishop_fn listen
 * @module net
 * @description Creates a TCP listener bound to the specified host and port.
 * @param host str - Host address to bind to (e.g., "127.0.0.1", "0.0.0.0")
 * @param port int - Port number to listen on
 * @returns net.TcpListener or err - The listener or an error if binding fails
 * @example
 * server := net.listen("127.0.0.1", 8080) or {
 *     print("Failed to listen:", err.message);
 *     return;
 * };
 */

/**
 * @bishop_fn connect
 * @module net
 * @description Establishes a TCP connection to the specified host and port.
 * @param host str - Host address to connect to
 * @param port int - Port number to connect to
 * @returns net.TcpStream or err - The connection or an error if connection fails
 * @example
 * conn := net.connect("example.com", 80) or return;
 * conn.write("GET / HTTP/1.1\r\nHost: example.com\r\n\r\n");
 */

/**
 * @bishop_fn udp_bind
 * @module net
 * @description Creates a UDP socket bound to the specified host and port.
 * @param host str - Host address to bind to
 * @param port int - Port number to bind to
 * @returns net.UdpSocket or err - The socket or an error if binding fails
 * @example
 * sock := net.udp_bind("0.0.0.0", 9000) or return;
 */

/**
 * @bishop_fn udp_connect
 * @module net
 * @description Creates a UDP socket connected to a specific remote address.
 * @param host str - Remote host address
 * @param port int - Remote port number
 * @returns net.UdpSocket or err - The connected socket or an error
 * @example
 * sock := net.udp_connect("192.168.1.1", 9000) or return;
 * sock.send("hello");  // no need to specify address
 */

/**
 * @bishop_fn resolve
 * @module net
 * @description Resolves a hostname to a list of IP addresses.
 * @param hostname str - Hostname to resolve
 * @returns List<str> or err - List of IP addresses or an error
 * @example
 * addrs := net.resolve("example.com") or return;
 * for addr in addrs {
 *     print(addr);
 * }
 */

/**
 * @bishop_fn reverse_lookup
 * @module net
 * @description Performs a reverse DNS lookup on an IP address.
 * @param ip str - IP address to look up
 * @returns str or err - Hostname or an error
 * @example
 * hostname := net.reverse_lookup("93.184.216.34") or return;
 * print(hostname);
 */

/**
 * @bishop_method accept
 * @type net.TcpListener
 * @description Accepts an incoming connection, returning a new TcpStream.
 * Blocks until a connection is available.
 * @returns net.TcpStream or err - The new connection or an error
 * @example
 * conn := server.accept() or continue;
 */

/**
 * @bishop_method close
 * @type net.TcpListener
 * @description Closes the listener and stops accepting connections.
 * Called automatically when `with` block exits.
 * @example
 * server.close();
 */

/**
 * @bishop_method read
 * @type net.TcpStream
 * @description Reads up to n bytes from the connection.
 * @param n int - Maximum number of bytes to read
 * @returns str or err - Data read or an error
 * @example
 * data := conn.read(1024) or return;
 */

/**
 * @bishop_method read_exact
 * @type net.TcpStream
 * @description Reads exactly n bytes from the connection.
 * @param n int - Exact number of bytes to read
 * @returns str or err - Data read or an error
 * @example
 * header := conn.read_exact(4) or return;
 */

/**
 * @bishop_method read_line
 * @type net.TcpStream
 * @description Reads a line (up to newline) from the connection.
 * @returns str or err - Line read (without newline) or an error
 * @example
 * line := conn.read_line() or return;
 */

/**
 * @bishop_method write
 * @type net.TcpStream
 * @description Writes data to the connection.
 * @param data str - Data to write
 * @returns int or err - Number of bytes written or an error
 * @example
 * conn.write("Hello, World!") or return;
 */

/**
 * @bishop_method flush
 * @type net.TcpStream
 * @description Flushes any buffered data to the connection.
 * @returns bool or err - True on success or an error
 * @example
 * conn.flush() or return;
 */

/**
 * @bishop_method close
 * @type net.TcpStream
 * @description Closes the connection.
 * Called automatically when `with` block exits.
 * @example
 * conn.close();
 */

/**
 * @bishop_method set_timeout
 * @type net.TcpStream
 * @description Sets read/write timeout in milliseconds.
 * @param ms int - Timeout in milliseconds (0 for no timeout)
 * @example
 * conn.set_timeout(5000);  // 5 second timeout
 */

/**
 * @bishop_method send_to
 * @type net.UdpSocket
 * @description Sends data to a specific address.
 * @param data str - Data to send
 * @param host str - Destination host
 * @param port int - Destination port
 * @returns int or err - Number of bytes sent or an error
 * @example
 * sock.send_to("hello", "192.168.1.1", 9000) or return;
 */

/**
 * @bishop_method recv_from
 * @type net.UdpSocket
 * @description Receives data and returns the sender's address.
 * @param n int - Maximum bytes to receive
 * @returns net.UdpPacket or err - Packet with data and sender info or an error
 * @example
 * pkt := sock.recv_from(1024) or return;
 * print("Received from", pkt.addr, ":", pkt.port);
 * print(pkt.data);
 */

/**
 * @bishop_method send
 * @type net.UdpSocket
 * @description Sends data to the connected address (for connected UDP sockets).
 * @param data str - Data to send
 * @returns int or err - Number of bytes sent or an error
 * @example
 * sock.send("hello") or return;
 */

/**
 * @bishop_method recv
 * @type net.UdpSocket
 * @description Receives data from the connected address.
 * @param n int - Maximum bytes to receive
 * @returns str or err - Data received or an error
 * @example
 * data := sock.recv(1024) or return;
 */

/**
 * @bishop_method close
 * @type net.UdpSocket
 * @description Closes the UDP socket.
 * Called automatically when `with` block exits.
 * @example
 * sock.close();
 */

#include "net.hpp"

using namespace std;

namespace bishop::stdlib {

/**
 * Creates the AST for the built-in net module.
 */
unique_ptr<Program> create_net_module() {
    auto program = make_unique<Program>();

    // TcpListener :: struct { }
    auto tcp_listener = make_unique<StructDef>();
    tcp_listener->name = "TcpListener";
    tcp_listener->visibility = Visibility::Public;
    program->structs.push_back(move(tcp_listener));

    // TcpStream :: struct { }
    auto tcp_stream = make_unique<StructDef>();
    tcp_stream->name = "TcpStream";
    tcp_stream->visibility = Visibility::Public;
    program->structs.push_back(move(tcp_stream));

    // UdpSocket :: struct { }
    auto udp_socket = make_unique<StructDef>();
    udp_socket->name = "UdpSocket";
    udp_socket->visibility = Visibility::Public;
    program->structs.push_back(move(udp_socket));

    // UdpPacket :: struct { data str, addr str, port int }
    auto udp_packet = make_unique<StructDef>();
    udp_packet->name = "UdpPacket";
    udp_packet->visibility = Visibility::Public;
    udp_packet->fields.push_back({"data", "str", ""});
    udp_packet->fields.push_back({"addr", "str", ""});
    udp_packet->fields.push_back({"port", "int", ""});
    program->structs.push_back(move(udp_packet));

    // fn listen(str host, int port) -> net.TcpListener or err
    auto listen_fn = make_unique<FunctionDef>();
    listen_fn->name = "listen";
    listen_fn->visibility = Visibility::Public;
    listen_fn->params.push_back({"str", "host"});
    listen_fn->params.push_back({"int", "port"});
    listen_fn->return_type = "net.TcpListener";
    listen_fn->error_type = "err";
    program->functions.push_back(move(listen_fn));

    // fn connect(str host, int port) -> net.TcpStream or err
    auto connect_fn = make_unique<FunctionDef>();
    connect_fn->name = "connect";
    connect_fn->visibility = Visibility::Public;
    connect_fn->params.push_back({"str", "host"});
    connect_fn->params.push_back({"int", "port"});
    connect_fn->return_type = "net.TcpStream";
    connect_fn->error_type = "err";
    program->functions.push_back(move(connect_fn));

    // fn udp_bind(str host, int port) -> net.UdpSocket or err
    auto udp_bind_fn = make_unique<FunctionDef>();
    udp_bind_fn->name = "udp_bind";
    udp_bind_fn->visibility = Visibility::Public;
    udp_bind_fn->params.push_back({"str", "host"});
    udp_bind_fn->params.push_back({"int", "port"});
    udp_bind_fn->return_type = "net.UdpSocket";
    udp_bind_fn->error_type = "err";
    program->functions.push_back(move(udp_bind_fn));

    // fn udp_connect(str host, int port) -> net.UdpSocket or err
    auto udp_connect_fn = make_unique<FunctionDef>();
    udp_connect_fn->name = "udp_connect";
    udp_connect_fn->visibility = Visibility::Public;
    udp_connect_fn->params.push_back({"str", "host"});
    udp_connect_fn->params.push_back({"int", "port"});
    udp_connect_fn->return_type = "net.UdpSocket";
    udp_connect_fn->error_type = "err";
    program->functions.push_back(move(udp_connect_fn));

    // fn resolve(str hostname) -> List<str> or err
    auto resolve_fn = make_unique<FunctionDef>();
    resolve_fn->name = "resolve";
    resolve_fn->visibility = Visibility::Public;
    resolve_fn->params.push_back({"str", "hostname"});
    resolve_fn->return_type = "List<str>";
    resolve_fn->error_type = "err";
    program->functions.push_back(move(resolve_fn));

    // fn reverse_lookup(str ip) -> str or err
    auto reverse_lookup_fn = make_unique<FunctionDef>();
    reverse_lookup_fn->name = "reverse_lookup";
    reverse_lookup_fn->visibility = Visibility::Public;
    reverse_lookup_fn->params.push_back({"str", "ip"});
    reverse_lookup_fn->return_type = "str";
    reverse_lookup_fn->error_type = "err";
    program->functions.push_back(move(reverse_lookup_fn));

    // TcpListener :: accept(self) -> net.TcpStream or err
    auto accept_method = make_unique<MethodDef>();
    accept_method->struct_name = "TcpListener";
    accept_method->name = "accept";
    accept_method->visibility = Visibility::Public;
    accept_method->params.push_back({"net.TcpListener", "self"});
    accept_method->return_type = "net.TcpStream";
    accept_method->error_type = "err";
    program->methods.push_back(move(accept_method));

    // TcpListener :: close(self)
    auto listener_close_method = make_unique<MethodDef>();
    listener_close_method->struct_name = "TcpListener";
    listener_close_method->name = "close";
    listener_close_method->visibility = Visibility::Public;
    listener_close_method->params.push_back({"net.TcpListener", "self"});
    program->methods.push_back(move(listener_close_method));

    // TcpStream :: read(self, int n) -> str or err
    auto read_method = make_unique<MethodDef>();
    read_method->struct_name = "TcpStream";
    read_method->name = "read";
    read_method->visibility = Visibility::Public;
    read_method->params.push_back({"net.TcpStream", "self"});
    read_method->params.push_back({"int", "n"});
    read_method->return_type = "str";
    read_method->error_type = "err";
    program->methods.push_back(move(read_method));

    // TcpStream :: read_exact(self, int n) -> str or err
    auto read_exact_method = make_unique<MethodDef>();
    read_exact_method->struct_name = "TcpStream";
    read_exact_method->name = "read_exact";
    read_exact_method->visibility = Visibility::Public;
    read_exact_method->params.push_back({"net.TcpStream", "self"});
    read_exact_method->params.push_back({"int", "n"});
    read_exact_method->return_type = "str";
    read_exact_method->error_type = "err";
    program->methods.push_back(move(read_exact_method));

    // TcpStream :: read_line(self) -> str or err
    auto read_line_method = make_unique<MethodDef>();
    read_line_method->struct_name = "TcpStream";
    read_line_method->name = "read_line";
    read_line_method->visibility = Visibility::Public;
    read_line_method->params.push_back({"net.TcpStream", "self"});
    read_line_method->return_type = "str";
    read_line_method->error_type = "err";
    program->methods.push_back(move(read_line_method));

    // TcpStream :: write(self, str data) -> int or err
    auto write_method = make_unique<MethodDef>();
    write_method->struct_name = "TcpStream";
    write_method->name = "write";
    write_method->visibility = Visibility::Public;
    write_method->params.push_back({"net.TcpStream", "self"});
    write_method->params.push_back({"str", "data"});
    write_method->return_type = "int";
    write_method->error_type = "err";
    program->methods.push_back(move(write_method));

    // TcpStream :: flush(self) -> bool or err
    auto flush_method = make_unique<MethodDef>();
    flush_method->struct_name = "TcpStream";
    flush_method->name = "flush";
    flush_method->visibility = Visibility::Public;
    flush_method->params.push_back({"net.TcpStream", "self"});
    flush_method->return_type = "bool";
    flush_method->error_type = "err";
    program->methods.push_back(move(flush_method));

    // TcpStream :: close(self)
    auto stream_close_method = make_unique<MethodDef>();
    stream_close_method->struct_name = "TcpStream";
    stream_close_method->name = "close";
    stream_close_method->visibility = Visibility::Public;
    stream_close_method->params.push_back({"net.TcpStream", "self"});
    program->methods.push_back(move(stream_close_method));

    // TcpStream :: set_timeout(self, int ms)
    auto set_timeout_method = make_unique<MethodDef>();
    set_timeout_method->struct_name = "TcpStream";
    set_timeout_method->name = "set_timeout";
    set_timeout_method->visibility = Visibility::Public;
    set_timeout_method->params.push_back({"net.TcpStream", "self"});
    set_timeout_method->params.push_back({"int", "ms"});
    program->methods.push_back(move(set_timeout_method));

    // UdpSocket :: send_to(self, str data, str host, int port) -> int or err
    auto send_to_method = make_unique<MethodDef>();
    send_to_method->struct_name = "UdpSocket";
    send_to_method->name = "send_to";
    send_to_method->visibility = Visibility::Public;
    send_to_method->params.push_back({"net.UdpSocket", "self"});
    send_to_method->params.push_back({"str", "data"});
    send_to_method->params.push_back({"str", "host"});
    send_to_method->params.push_back({"int", "port"});
    send_to_method->return_type = "int";
    send_to_method->error_type = "err";
    program->methods.push_back(move(send_to_method));

    // UdpSocket :: recv_from(self, int n) -> net.UdpPacket or err
    auto recv_from_method = make_unique<MethodDef>();
    recv_from_method->struct_name = "UdpSocket";
    recv_from_method->name = "recv_from";
    recv_from_method->visibility = Visibility::Public;
    recv_from_method->params.push_back({"net.UdpSocket", "self"});
    recv_from_method->params.push_back({"int", "n"});
    recv_from_method->return_type = "net.UdpPacket";
    recv_from_method->error_type = "err";
    program->methods.push_back(move(recv_from_method));

    // UdpSocket :: send(self, str data) -> int or err
    auto send_method = make_unique<MethodDef>();
    send_method->struct_name = "UdpSocket";
    send_method->name = "send";
    send_method->visibility = Visibility::Public;
    send_method->params.push_back({"net.UdpSocket", "self"});
    send_method->params.push_back({"str", "data"});
    send_method->return_type = "int";
    send_method->error_type = "err";
    program->methods.push_back(move(send_method));

    // UdpSocket :: recv(self, int n) -> str or err
    auto recv_method = make_unique<MethodDef>();
    recv_method->struct_name = "UdpSocket";
    recv_method->name = "recv";
    recv_method->visibility = Visibility::Public;
    recv_method->params.push_back({"net.UdpSocket", "self"});
    recv_method->params.push_back({"int", "n"});
    recv_method->return_type = "str";
    recv_method->error_type = "err";
    program->methods.push_back(move(recv_method));

    // UdpSocket :: close(self)
    auto udp_close_method = make_unique<MethodDef>();
    udp_close_method->struct_name = "UdpSocket";
    udp_close_method->name = "close";
    udp_close_method->visibility = Visibility::Public;
    udp_close_method->params.push_back({"net.UdpSocket", "self"});
    program->methods.push_back(move(udp_close_method));

    return program;
}

/**
 * Returns empty - net.hpp is included at the top of generated code
 * for precompiled header support.
 */
string generate_net_runtime() {
    return "";
}

}  // namespace bishop::stdlib
