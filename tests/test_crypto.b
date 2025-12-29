// ============================================
// Crypto Module Tests
// ============================================

import crypto;

// ============================================
// Hash Functions
// ============================================

fn test_md5_basic() {
    hash := crypto.md5("hello") or return;
    assert_eq(hash, "5d41402abc4b2a76b9719d911017c592");
}

fn test_md5_empty() {
    hash := crypto.md5("") or return;
    assert_eq(hash, "d41d8cd98f00b204e9800998ecf8427e");
}

fn test_sha1_basic() {
    hash := crypto.sha1("hello") or return;
    assert_eq(hash, "aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d");
}

fn test_sha1_empty() {
    hash := crypto.sha1("") or return;
    assert_eq(hash, "da39a3ee5e6b4b0d3255bfef95601890afd80709");
}

fn test_sha256_basic() {
    hash := crypto.sha256("hello") or return;
    assert_eq(hash, "2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824");
}

fn test_sha256_empty() {
    hash := crypto.sha256("") or return;
    assert_eq(hash, "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
}

fn test_sha512_basic() {
    hash := crypto.sha512("hello") or return;
    assert_eq(hash, "9b71d224bd62f3785d96d46ad3ea3d73319bfbc2890caadae2dff72519673ca72323c3d99ba5c11d7c7acc6e14b8c5da0c4663475c2e5c3adef46f73bcdec043");
}

fn test_sha512_empty() {
    hash := crypto.sha512("") or return;
    assert_eq(hash, "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e");
}

// ============================================
// HMAC
// ============================================

fn test_hmac_sha256_basic() {
    hmac := crypto.hmac_sha256("key", "hello") or return;
    assert_eq(hmac, "9307b3b915efb5171ff14d8cb55fbcc798c6c0ef1456d66ded1a6aa723a58b7b");
}

fn test_hmac_sha256_empty_data() {
    hmac := crypto.hmac_sha256("key", "") or return;
    assert_eq(hmac, "5d5d139563c95b5967b9bd9a8c9b233a9dedb45072794cd232dc1b74832607d0");
}

// ============================================
// Base64 Encoding/Decoding
// ============================================

fn test_base64_encode_basic() {
    encoded := crypto.base64_encode("Hello, World!");
    assert_eq(encoded, "SGVsbG8sIFdvcmxkIQ==");
}

fn test_base64_encode_empty() {
    encoded := crypto.base64_encode("");
    assert_eq(encoded, "");
}

fn test_base64_decode_basic() {
    decoded := crypto.base64_decode("SGVsbG8sIFdvcmxkIQ==") or return;
    assert_eq(decoded, "Hello, World!");
}

fn test_base64_decode_empty() {
    decoded := crypto.base64_decode("") or return;
    assert_eq(decoded, "");
}

fn test_base64_roundtrip() {
    original := "The quick brown fox jumps over the lazy dog";
    encoded := crypto.base64_encode(original);
    decoded := crypto.base64_decode(encoded) or return;
    assert_eq(decoded, original);
}

fn test_base64_decode_invalid_length() {
    // Invalid length (not multiple of 4) should fail
    passed := false;

    result := crypto.base64_decode("abc") or {
        passed = true;
        return;
    };

    assert_eq(passed, true);
}

fn test_base64_decode_invalid_chars() {
    // Invalid characters should fail
    passed := false;

    result := crypto.base64_decode("!@#$") or {
        passed = true;
        return;
    };

    assert_eq(passed, true);
}

fn test_base64_decode_invalid_padding() {
    // Padding only (no data) should fail - first chars are invalid
    passed := false;

    result := crypto.base64_decode("====") or {
        passed = true;
        return;
    };

    assert_eq(passed, true);
}

// ============================================
// Hex Encoding/Decoding
// ============================================

fn test_hex_encode_basic() {
    encoded := crypto.hex_encode("hello");
    assert_eq(encoded, "68656c6c6f");
}

fn test_hex_encode_empty() {
    encoded := crypto.hex_encode("");
    assert_eq(encoded, "");
}

fn test_hex_decode_basic() {
    decoded := crypto.hex_decode("68656c6c6f") or return;
    assert_eq(decoded, "hello");
}

fn test_hex_decode_empty() {
    decoded := crypto.hex_decode("") or return;
    assert_eq(decoded, "");
}

fn test_hex_roundtrip() {
    original := "binary data test";
    encoded := crypto.hex_encode(original);
    decoded := crypto.hex_decode(encoded) or return;
    assert_eq(decoded, original);
}

fn test_hex_decode_invalid_length() {
    // Odd-length hex string should fail
    passed := false;

    result := crypto.hex_decode("abc") or {
        passed = true;
        return;
    };

    assert_eq(passed, true);
}

fn test_hex_decode_invalid_chars() {
    // Non-hex characters should fail
    passed := false;

    result := crypto.hex_decode("ghij") or {
        passed = true;
        return;
    };

    assert_eq(passed, true);
}

// ============================================
// UUID Generation
// ============================================

fn test_uuid_v4_format() {
    id := crypto.uuid() or return;
    // UUID v4 format: xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
    // where y is 8, 9, a, or b
    assert_eq(id.length(), 36);
    assert_eq(id.at(8), '-');
    assert_eq(id.at(13), '-');
    assert_eq(id.at(14), '4');
    assert_eq(id.at(18), '-');
    assert_eq(id.at(23), '-');
}

fn test_uuid_v4_uniqueness() {
    id1 := crypto.uuid() or return;
    id2 := crypto.uuid() or return;
    // Two UUIDs should be different (statistically guaranteed)
    assert_eq(id1 == id2, false);
}

fn test_uuid_v5_deterministic() {
    // Same namespace and name should produce same UUID
    id1 := crypto.uuid_v5("namespace", "name") or return;
    id2 := crypto.uuid_v5("namespace", "name") or return;
    assert_eq(id1, id2);
}

fn test_uuid_v5_different_names() {
    id1 := crypto.uuid_v5("namespace", "name1") or return;
    id2 := crypto.uuid_v5("namespace", "name2") or return;
    assert_eq(id1 == id2, false);
}

fn test_uuid_v5_format() {
    id := crypto.uuid_v5("namespace", "name") or return;
    assert_eq(id.length(), 36);
    assert_eq(id.at(8), '-');
    assert_eq(id.at(13), '-');
    assert_eq(id.at(14), '5');
    assert_eq(id.at(18), '-');
    assert_eq(id.at(23), '-');
}

// ============================================
// Random Bytes
// ============================================

fn test_random_bytes_length() {
    bytes := crypto.random_bytes(32) or return;
    assert_eq(bytes.length(), 32);
}

fn test_random_bytes_zero() {
    bytes := crypto.random_bytes(0) or return;
    assert_eq(bytes.length(), 0);
}

fn test_random_bytes_uniqueness() {
    bytes1 := crypto.random_bytes(16) or return;
    bytes2 := crypto.random_bytes(16) or return;
    // Statistically, two random byte arrays should be different
    same := true;
    i := 0;

    while i < 16 {
        if bytes1.get(i) != bytes2.get(i) {
            same = false;
        }

        i = i + 1;
    }

    assert_eq(same, false);
}

fn test_random_bytes_non_zero() {
    // Verify at least some bytes are non-zero to catch obvious RNG failures
    bytes := crypto.random_bytes(32) or return;
    has_nonzero := false;
    i := 0;

    while i < 32 {
        if bytes.get(i) != 0 {
            has_nonzero = true;
        }

        i = i + 1;
    }

    assert_eq(has_nonzero, true);
}

fn test_random_bytes_negative() {
    // Negative count should fail
    passed := false;

    result := crypto.random_bytes(-1) or {
        passed = true;
        return;
    };

    assert_eq(passed, true);
}
