// TCP Echo Client Example
// Run with: bishop run examples/echo_client.b
// Connects to an echo server and tests TCP client functionality

import net;

fn main() {
    host := "tcpbin.com";
    port := 4242;

    print("Connecting to " + host + ":" + port);

    conn := net.connect(host, port) or {
        print("Connection failed: " + err.message);
        return;
    };

    print("Connected!");

    message := "Hello from Bishop!";
    print("Sending: " + message);

    conn.write(message + "\n") or {
        print("Write failed: " + err.message);
        conn.close();
        return;
    };

    response := conn.read_line() or {
        print("Read failed: " + err.message);
        conn.close();
        return;
    };

    print("Received: " + response);

    conn.close();
    print("Connection closed");
}
