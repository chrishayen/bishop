/**
 * @file net.hpp
 * @brief Bishop networking runtime library.
 *
 * Provides TCP, UDP, and DNS functionality for Bishop programs.
 * Uses boost::fibers with Asio integration for async I/O.
 *
 * This header is included when programs import the net module.
 */

#pragma once

#include <bishop/std.hpp>
#include <boost/fiber/all.hpp>
#include <boost/asio.hpp>
#include <bishop/fiber_asio/yield.hpp>

namespace bishop::rt {

/**
 * Get the global io_context. Only available in networking programs.
 * Defined in runtime.cpp.
 */
boost::asio::io_context& io_context();

}  // namespace bishop::rt

namespace net {

// Forward declarations
struct TcpStream;
struct UdpPacket;

/**
 * UDP packet with data and sender information.
 */
struct UdpPacket {
    std::string data;
    std::string addr;
    int port = 0;
};

/**
 * TCP stream for reading and writing data.
 */
struct TcpStream {
    std::shared_ptr<boost::asio::ip::tcp::socket> socket;
    std::string read_buffer;
    int timeout_ms = 0;
    bool closed = false;

    TcpStream() = default;
    TcpStream(std::shared_ptr<boost::asio::ip::tcp::socket> sock) : socket(sock) {}

    /**
     * Reads up to n bytes from the connection.
     */
    bishop::rt::Result<std::string> read(int n) {
        if (closed || !socket) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("Connection is closed")
            );
        }

        try {
            std::vector<char> buffer(n);
            boost::system::error_code ec;
            size_t bytes_read = socket->async_read_some(
                boost::asio::buffer(buffer),
                boost::fibers::asio::yield[ec]
            );

            if (ec == boost::asio::error::eof) {
                return bishop::rt::Result<std::string>::ok(std::string());
            }

            if (ec) {
                return bishop::rt::Result<std::string>::error(
                    bishop::rt::Error("Read failed: " + ec.message())
                );
            }

            return bishop::rt::Result<std::string>::ok(std::string(buffer.data(), bytes_read));
        } catch (const boost::system::system_error& e) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("Read failed: " + std::string(e.what()))
            );
        }
    }

    /**
     * Reads exactly n bytes from the connection.
     */
    bishop::rt::Result<std::string> read_exact(int n) {
        if (closed || !socket) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("Connection is closed")
            );
        }

        try {
            std::vector<char> buffer(n);
            boost::system::error_code ec;
            boost::asio::async_read(
                *socket,
                boost::asio::buffer(buffer),
                boost::fibers::asio::yield[ec]
            );

            if (ec) {
                return bishop::rt::Result<std::string>::error(
                    bishop::rt::Error("Read failed: " + ec.message())
                );
            }

            return bishop::rt::Result<std::string>::ok(std::string(buffer.data(), n));
        } catch (const boost::system::system_error& e) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("Read failed: " + std::string(e.what()))
            );
        }
    }

    /**
     * Reads a line from the connection.
     */
    bishop::rt::Result<std::string> read_line() {
        if (closed || !socket) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("Connection is closed")
            );
        }

        try {
            boost::asio::streambuf buffer;
            boost::system::error_code ec;
            boost::asio::async_read_until(
                *socket,
                buffer,
                '\n',
                boost::fibers::asio::yield[ec]
            );

            if (ec && ec != boost::asio::error::eof) {
                return bishop::rt::Result<std::string>::error(
                    bishop::rt::Error("Read failed: " + ec.message())
                );
            }

            std::istream is(&buffer);
            std::string line;
            std::getline(is, line);

            // Remove trailing \r if present
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            return bishop::rt::Result<std::string>::ok(line);
        } catch (const boost::system::system_error& e) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("Read failed: " + std::string(e.what()))
            );
        }
    }

    /**
     * Writes data to the connection.
     */
    bishop::rt::Result<int> write(const std::string& data) {
        if (closed || !socket) {
            return bishop::rt::Result<int>::error(
                bishop::rt::Error("Connection is closed")
            );
        }

        try {
            boost::system::error_code ec;
            size_t bytes_written = boost::asio::async_write(
                *socket,
                boost::asio::buffer(data),
                boost::fibers::asio::yield[ec]
            );

            if (ec) {
                return bishop::rt::Result<int>::error(
                    bishop::rt::Error("Write failed: " + ec.message())
                );
            }

            return bishop::rt::Result<int>::ok(static_cast<int>(bytes_written));
        } catch (const boost::system::system_error& e) {
            return bishop::rt::Result<int>::error(
                bishop::rt::Error("Write failed: " + std::string(e.what()))
            );
        }
    }

    /**
     * Flushes any buffered data.
     */
    bishop::rt::Result<bool> flush() {
        if (closed || !socket) {
            return bishop::rt::Result<bool>::error(
                bishop::rt::Error("Connection is closed")
            );
        }

        // TCP sockets in ASIO don't have buffering at this level
        // This is a no-op that always succeeds
        return bishop::rt::Result<bool>::ok(true);
    }

    /**
     * Closes the connection.
     */
    void close() {
        if (!closed && socket) {
            boost::system::error_code ec;
            socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
            socket->close(ec);
            closed = true;
        }
    }

    /**
     * Sets read/write timeout in milliseconds.
     */
    void set_timeout(int ms) {
        timeout_ms = ms;
        // Note: Boost.Asio doesn't have built-in socket timeouts.
        // For full timeout support, we would need to use async_wait with a timer.
        // This stores the value for potential future use.
    }
};

/**
 * TCP listener for accepting incoming connections.
 */
struct TcpListener {
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor;
    bool closed = false;

    TcpListener() = default;
    TcpListener(std::shared_ptr<boost::asio::ip::tcp::acceptor> acc) : acceptor(acc) {}

    /**
     * Accepts an incoming connection.
     */
    bishop::rt::Result<TcpStream> accept() {
        if (closed || !acceptor) {
            return bishop::rt::Result<TcpStream>::error(
                bishop::rt::Error("Listener is closed")
            );
        }

        try {
            auto socket = std::make_shared<boost::asio::ip::tcp::socket>(bishop::rt::io_context());
            boost::system::error_code ec;
            acceptor->async_accept(*socket, boost::fibers::asio::yield[ec]);

            if (ec) {
                return bishop::rt::Result<TcpStream>::error(
                    bishop::rt::Error("Accept failed: " + ec.message())
                );
            }

            return bishop::rt::Result<TcpStream>::ok(TcpStream(socket));
        } catch (const boost::system::system_error& e) {
            return bishop::rt::Result<TcpStream>::error(
                bishop::rt::Error("Accept failed: " + std::string(e.what()))
            );
        }
    }

    /**
     * Closes the listener.
     */
    void close() {
        if (!closed && acceptor) {
            boost::system::error_code ec;
            acceptor->close(ec);
            closed = true;
        }
    }
};

/**
 * UDP socket for sending and receiving datagrams.
 */
struct UdpSocket {
    std::shared_ptr<boost::asio::ip::udp::socket> socket;
    boost::asio::ip::udp::endpoint remote_endpoint;
    bool connected = false;
    bool closed = false;

    UdpSocket() = default;
    UdpSocket(std::shared_ptr<boost::asio::ip::udp::socket> sock) : socket(sock) {}

    /**
     * Sends data to a specific address.
     */
    bishop::rt::Result<int> send_to(const std::string& data, const std::string& host, int port) {
        if (closed || !socket) {
            return bishop::rt::Result<int>::error(
                bishop::rt::Error("Socket is closed")
            );
        }

        try {
            boost::asio::ip::udp::endpoint endpoint(
                boost::asio::ip::make_address(host),
                static_cast<boost::asio::ip::port_type>(port)
            );

            boost::system::error_code ec;
            size_t bytes_sent = socket->async_send_to(
                boost::asio::buffer(data),
                endpoint,
                boost::fibers::asio::yield[ec]
            );

            if (ec) {
                return bishop::rt::Result<int>::error(
                    bishop::rt::Error("Send failed: " + ec.message())
                );
            }

            return bishop::rt::Result<int>::ok(static_cast<int>(bytes_sent));
        } catch (const boost::system::system_error& e) {
            return bishop::rt::Result<int>::error(
                bishop::rt::Error("Send failed: " + std::string(e.what()))
            );
        }
    }

    /**
     * Receives data and returns the sender's address.
     */
    bishop::rt::Result<UdpPacket> recv_from(int n) {
        if (closed || !socket) {
            return bishop::rt::Result<UdpPacket>::error(
                bishop::rt::Error("Socket is closed")
            );
        }

        try {
            std::vector<char> buffer(n);
            boost::asio::ip::udp::endpoint sender_endpoint;
            boost::system::error_code ec;

            size_t bytes_received = socket->async_receive_from(
                boost::asio::buffer(buffer),
                sender_endpoint,
                boost::fibers::asio::yield[ec]
            );

            if (ec) {
                return bishop::rt::Result<UdpPacket>::error(
                    bishop::rt::Error("Receive failed: " + ec.message())
                );
            }

            UdpPacket packet;
            packet.data = std::string(buffer.data(), bytes_received);
            packet.addr = sender_endpoint.address().to_string();
            packet.port = sender_endpoint.port();

            return bishop::rt::Result<UdpPacket>::ok(packet);
        } catch (const boost::system::system_error& e) {
            return bishop::rt::Result<UdpPacket>::error(
                bishop::rt::Error("Receive failed: " + std::string(e.what()))
            );
        }
    }

    /**
     * Sends data to the connected address.
     */
    bishop::rt::Result<int> send(const std::string& data) {
        if (closed || !socket) {
            return bishop::rt::Result<int>::error(
                bishop::rt::Error("Socket is closed")
            );
        }

        if (!connected) {
            return bishop::rt::Result<int>::error(
                bishop::rt::Error("Socket is not connected")
            );
        }

        try {
            boost::system::error_code ec;
            size_t bytes_sent = socket->async_send(
                boost::asio::buffer(data),
                boost::fibers::asio::yield[ec]
            );

            if (ec) {
                return bishop::rt::Result<int>::error(
                    bishop::rt::Error("Send failed: " + ec.message())
                );
            }

            return bishop::rt::Result<int>::ok(static_cast<int>(bytes_sent));
        } catch (const boost::system::system_error& e) {
            return bishop::rt::Result<int>::error(
                bishop::rt::Error("Send failed: " + std::string(e.what()))
            );
        }
    }

    /**
     * Receives data from the connected address.
     */
    bishop::rt::Result<std::string> recv(int n) {
        if (closed || !socket) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("Socket is closed")
            );
        }

        if (!connected) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("Socket is not connected")
            );
        }

        try {
            std::vector<char> buffer(n);
            boost::system::error_code ec;

            size_t bytes_received = socket->async_receive(
                boost::asio::buffer(buffer),
                boost::fibers::asio::yield[ec]
            );

            if (ec) {
                return bishop::rt::Result<std::string>::error(
                    bishop::rt::Error("Receive failed: " + ec.message())
                );
            }

            return bishop::rt::Result<std::string>::ok(std::string(buffer.data(), bytes_received));
        } catch (const boost::system::system_error& e) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("Receive failed: " + std::string(e.what()))
            );
        }
    }

    /**
     * Closes the UDP socket.
     */
    void close() {
        if (!closed && socket) {
            boost::system::error_code ec;
            socket->close(ec);
            closed = true;
        }
    }
};

// ============================================================================
// Module-level functions
// ============================================================================

/**
 * Creates a TCP listener bound to the specified host and port.
 */
inline bishop::rt::Result<TcpListener> listen(const std::string& host, int port) {
    try {
        auto acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(bishop::rt::io_context());
        boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::make_address(host),
            static_cast<boost::asio::ip::port_type>(port)
        );

        acceptor->open(endpoint.protocol());
        acceptor->set_option(boost::asio::socket_base::reuse_address(true));
        acceptor->bind(endpoint);
        acceptor->listen();

        return bishop::rt::Result<TcpListener>::ok(TcpListener(acceptor));
    } catch (const boost::system::system_error& e) {
        return bishop::rt::Result<TcpListener>::error(
            bishop::rt::Error("Failed to listen: " + std::string(e.what()))
        );
    }
}

/**
 * Establishes a TCP connection to the specified host and port.
 */
inline bishop::rt::Result<TcpStream> connect(const std::string& host, int port) {
    try {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(bishop::rt::io_context());

        boost::asio::ip::tcp::resolver resolver(bishop::rt::io_context());
        boost::system::error_code resolve_ec;
        auto endpoints = resolver.async_resolve(
            host,
            std::to_string(port),
            boost::fibers::asio::yield[resolve_ec]
        );

        if (resolve_ec) {
            return bishop::rt::Result<TcpStream>::error(
                bishop::rt::Error("DNS resolution failed: " + resolve_ec.message())
            );
        }

        boost::system::error_code connect_ec;
        boost::asio::async_connect(*socket, endpoints, boost::fibers::asio::yield[connect_ec]);

        if (connect_ec) {
            return bishop::rt::Result<TcpStream>::error(
                bishop::rt::Error("Connection failed: " + connect_ec.message())
            );
        }

        return bishop::rt::Result<TcpStream>::ok(TcpStream(socket));
    } catch (const boost::system::system_error& e) {
        return bishop::rt::Result<TcpStream>::error(
            bishop::rt::Error("Connection failed: " + std::string(e.what()))
        );
    }
}

/**
 * Creates a UDP socket bound to the specified host and port.
 */
inline bishop::rt::Result<UdpSocket> udp_bind(const std::string& host, int port) {
    try {
        auto socket = std::make_shared<boost::asio::ip::udp::socket>(bishop::rt::io_context());
        boost::asio::ip::udp::endpoint endpoint(
            boost::asio::ip::make_address(host),
            static_cast<boost::asio::ip::port_type>(port)
        );

        socket->open(endpoint.protocol());
        socket->set_option(boost::asio::socket_base::reuse_address(true));
        socket->bind(endpoint);

        return bishop::rt::Result<UdpSocket>::ok(UdpSocket(socket));
    } catch (const boost::system::system_error& e) {
        return bishop::rt::Result<UdpSocket>::error(
            bishop::rt::Error("Failed to bind UDP socket: " + std::string(e.what()))
        );
    }
}

/**
 * Creates a UDP socket connected to a specific remote address.
 */
inline bishop::rt::Result<UdpSocket> udp_connect(const std::string& host, int port) {
    try {
        auto socket = std::make_shared<boost::asio::ip::udp::socket>(bishop::rt::io_context());
        boost::asio::ip::udp::endpoint endpoint(
            boost::asio::ip::make_address(host),
            static_cast<boost::asio::ip::port_type>(port)
        );

        socket->open(boost::asio::ip::udp::v4());
        socket->connect(endpoint);

        UdpSocket result(socket);
        result.connected = true;
        result.remote_endpoint = endpoint;

        return bishop::rt::Result<UdpSocket>::ok(result);
    } catch (const boost::system::system_error& e) {
        return bishop::rt::Result<UdpSocket>::error(
            bishop::rt::Error("Failed to connect UDP socket: " + std::string(e.what()))
        );
    }
}

/**
 * Resolves a hostname to a list of IP addresses.
 */
inline bishop::rt::Result<std::vector<std::string>> resolve(const std::string& hostname) {
    try {
        boost::asio::ip::tcp::resolver resolver(bishop::rt::io_context());
        boost::system::error_code ec;
        auto results = resolver.async_resolve(hostname, "", boost::fibers::asio::yield[ec]);

        if (ec) {
            return bishop::rt::Result<std::vector<std::string>>::error(
                bishop::rt::Error("DNS resolution failed: " + ec.message())
            );
        }

        std::vector<std::string> addresses;

        for (const auto& entry : results) {
            addresses.push_back(entry.endpoint().address().to_string());
        }

        return bishop::rt::Result<std::vector<std::string>>::ok(addresses);
    } catch (const boost::system::system_error& e) {
        return bishop::rt::Result<std::vector<std::string>>::error(
            bishop::rt::Error("DNS resolution failed: " + std::string(e.what()))
        );
    }
}

/**
 * Performs a reverse DNS lookup on an IP address.
 */
inline bishop::rt::Result<std::string> reverse_lookup(const std::string& ip) {
    try {
        boost::asio::ip::tcp::resolver resolver(bishop::rt::io_context());
        boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::make_address(ip),
            0
        );

        boost::system::error_code ec;
        auto results = resolver.async_resolve(endpoint, boost::fibers::asio::yield[ec]);

        if (ec) {
            return bishop::rt::Result<std::string>::error(
                bishop::rt::Error("Reverse lookup failed: " + ec.message())
            );
        }

        for (const auto& entry : results) {
            return bishop::rt::Result<std::string>::ok(entry.host_name());
        }

        return bishop::rt::Result<std::string>::error(
            bishop::rt::Error("No hostname found for " + ip)
        );
    } catch (const boost::system::system_error& e) {
        return bishop::rt::Result<std::string>::error(
            bishop::rt::Error("Reverse lookup failed: " + std::string(e.what()))
        );
    }
}

}  // namespace net
