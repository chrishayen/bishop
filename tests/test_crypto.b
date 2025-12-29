// Tests for the crypto module

import crypto;

// Hashing tests

fn test_md5() {
    hash := crypto.md5("Hello, World!");
    assert_eq("65a8e27d8879283831b664bd8b7f0ad4", hash);
}

fn test_md5_empty() {
    hash := crypto.md5("");
    assert_eq("d41d8cd98f00b204e9800998ecf8427e", hash);
}

fn test_sha1() {
    hash := crypto.sha1("Hello, World!");
    assert_eq("0a0a9f2a6772942557ab5355d76af442f8f65e01", hash);
}

fn test_sha1_empty() {
    hash := crypto.sha1("");
    assert_eq("da39a3ee5e6b4b0d3255bfef95601890afd80709", hash);
}

fn test_sha256() {
    hash := crypto.sha256("Hello, World!");
    assert_eq("dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f", hash);
}

fn test_sha256_empty() {
    hash := crypto.sha256("");
    assert_eq("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855", hash);
}

fn test_sha512() {
    hash := crypto.sha512("Hello, World!");
    expected := "374d794a95cdcfd8b35993185fef9ba368f160d8daf432d08ba9f1ed1e5abe6cc69291e0fa2fe0006a52570ef18c19def4e617c33ce52ef0a6e5fbe318cb0387";
    assert_eq(expected, hash);
}

fn test_sha512_empty() {
    hash := crypto.sha512("");
    expected := "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e";
    assert_eq(expected, hash);
}

// HMAC tests

fn test_hmac_sha256() {
    mac := crypto.hmac_sha256("secret", "message");
    assert_eq("8b5f48702995c1598c573db1e21866a9b825d4a794d169d7060a03605796360b", mac);
}

fn test_hmac_sha256_empty_message() {
    mac := crypto.hmac_sha256("key", "");
    assert_eq("5d5d139563c95b5967b9bd9a8c9b233a9dedb45072794cd232dc1b74832607d0", mac);
}

// Base64 tests

fn test_base64_encode() {
    encoded := crypto.base64_encode("Hello, World!");
    assert_eq("SGVsbG8sIFdvcmxkIQ==", encoded);
}

fn test_base64_encode_empty() {
    encoded := crypto.base64_encode("");
    assert_eq("", encoded);
}

fn test_base64_decode() {
    decoded := crypto.base64_decode("SGVsbG8sIFdvcmxkIQ==") or {
        print("decode error");
        return;
    };
    assert_eq("Hello, World!", decoded);
}

fn test_base64_decode_empty() {
    decoded := crypto.base64_decode("") or {
        print("decode error");
        return;
    };
    assert_eq("", decoded);
}

fn test_base64_roundtrip() {
    original := "The quick brown fox jumps over the lazy dog";
    encoded := crypto.base64_encode(original);
    decoded := crypto.base64_decode(encoded) or return;
    assert_eq(original, decoded);
}

// Hex encoding tests

fn test_hex_encode() {
    hex := crypto.hex_encode("Hi");
    assert_eq("4869", hex);
}

fn test_hex_encode_empty() {
    hex := crypto.hex_encode("");
    assert_eq("", hex);
}

fn test_hex_decode() {
    decoded := crypto.hex_decode("4869") or {
        print("decode error");
        return;
    };
    assert_eq("Hi", decoded);
}

fn test_hex_decode_empty() {
    decoded := crypto.hex_decode("") or {
        print("decode error");
        return;
    };
    assert_eq("", decoded);
}

fn test_hex_roundtrip() {
    original := "Test data 123";
    hex := crypto.hex_encode(original);
    decoded := crypto.hex_decode(hex) or return;
    assert_eq(original, decoded);
}

// UUID tests

fn test_uuid_format() {
    id := crypto.uuid();
    assert_eq(36, id.length());
    assert_eq('-', id.at(8));
    assert_eq('-', id.at(13));
    assert_eq('-', id.at(18));
    assert_eq('-', id.at(23));
}

fn test_uuid_v4_version() {
    id := crypto.uuid();
    version := id.at(14);
    assert_eq('4', version);
}

fn test_uuid_uniqueness() {
    id1 := crypto.uuid();
    id2 := crypto.uuid();
    assert_eq(false, id1 == id2);
}

fn test_uuid_v5() {
    id := crypto.uuid_v5("dns", "example.com");
    assert_eq(36, id.length());
    version := id.at(14);
    assert_eq('5', version);
}

fn test_uuid_v5_deterministic() {
    id1 := crypto.uuid_v5("dns", "example.com");
    id2 := crypto.uuid_v5("dns", "example.com");
    assert_eq(id1, id2);
}

fn test_uuid_v5_different_names() {
    id1 := crypto.uuid_v5("dns", "example.com");
    id2 := crypto.uuid_v5("dns", "example.org");
    assert_eq(false, id1 == id2);
}

// Random bytes tests

fn test_random_bytes_length() {
    bytes := crypto.random_bytes(32);
    assert_eq(32, bytes.length());
}

fn test_random_bytes_zero() {
    bytes := crypto.random_bytes(0);
    assert_eq(0, bytes.length());
}

fn test_random_bytes_range() {
    bytes := crypto.random_bytes(10);

    for b in bytes {
        assert_eq(true, b >= 0);
        assert_eq(true, b <= 255);
    }
}
