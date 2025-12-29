// DNS Resolution Example
// Run with: bishop run examples/resolve.b
// Demonstrates DNS resolution and reverse lookup

import net;

fn main() {
    hostname := "example.com";

    print("Resolving " + hostname + "...");

    addrs := net.resolve(hostname) or {
        print("DNS resolution failed: " + err.message);
        return;
    };

    print("IP addresses for " + hostname + ":");

    for addr in addrs {
        print("  " + addr);
    }

    // Try reverse lookup on the first address
    if addrs.length() > 0 {
        first_addr := addrs.first();
        print("\nReverse lookup for " + first_addr + "...");

        name := net.reverse_lookup(first_addr) or {
            print("Reverse lookup failed: " + err.message);
            return;
        };

        print("Hostname: " + name);
    }
}
