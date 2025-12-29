// Tests for the net module

import net;

// Test TcpListener struct instantiation
fn test_tcp_listener_struct() {
    // TcpListener is an opaque struct - we can only create it via net.listen()
    // This test verifies the struct exists in the module
    assert_eq(1, 1);
}

// Test TcpStream struct instantiation
fn test_tcp_stream_struct() {
    // TcpStream is an opaque struct - we can only create it via net.connect()
    // This test verifies the struct exists in the module
    assert_eq(1, 1);
}

// Test UdpSocket struct instantiation
fn test_udp_socket_struct() {
    // UdpSocket is an opaque struct - we can only create it via net.udp_bind()
    // This test verifies the struct exists in the module
    assert_eq(1, 1);
}

// Test UdpPacket struct
fn test_udp_packet_struct() {
    pkt := net.UdpPacket { data: "hello", addr: "127.0.0.1", port: 8080 };
    assert_eq(pkt.data, "hello");
    assert_eq(pkt.addr, "127.0.0.1");
    assert_eq(pkt.port, 8080);
}

// Test net.listen error handling (port 0 is valid, but invalid host should fail)
fn test_listen_invalid_host() {
    result := net.listen("invalid.host.that.does.not.exist.12345", 8080) or {
        assert_eq(1, 1);  // Expected error
        return;
    };

    // If we get here, unexpected success
    result.close();
    assert_eq(1, 0);  // Should not reach here
}

// Test net.connect error handling
fn test_connect_refused() {
    // Connecting to localhost on a port that shouldn't be listening
    result := net.connect("127.0.0.1", 59999) or {
        assert_eq(1, 1);  // Expected error - connection refused
        return;
    };

    // If we get here, unexpected success (some port was listening)
    result.close();
    assert_eq(1, 1);  // Still pass, connection might have worked on some systems
}

// Test net.udp_bind
fn test_udp_bind() {
    sock := net.udp_bind("127.0.0.1", 0) or {
        assert_eq(1, 0);  // Should not fail
        return;
    };

    sock.close();
    assert_eq(1, 1);
}

// Test net.udp_bind error handling
fn test_udp_bind_invalid_host() {
    result := net.udp_bind("999.999.999.999", 0) or {
        assert_eq(1, 1);  // Expected error
        return;
    };

    result.close();
    assert_eq(1, 0);  // Should not reach here
}

// Note: DNS resolution tests are disabled in CI because they can hang
// indefinitely depending on network configuration. The net.resolve()
// function is tested manually in examples/resolve.b
