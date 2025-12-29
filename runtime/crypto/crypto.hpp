/**
 * @file crypto.hpp
 * @brief Bishop crypto runtime library.
 *
 * Provides cryptographic utilities for Bishop programs.
 * Uses OpenSSL for all cryptographic operations.
 * This header is included when programs import the crypto module.
 */

#pragma once

#include <bishop/std.hpp>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <stdexcept>

namespace crypto {

/**
 * Converts a byte array to a lowercase hex string.
 */
inline std::string bytes_to_hex(const unsigned char* data, size_t len) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    for (size_t i = 0; i < len; i++) {
        oss << std::setw(2) << static_cast<int>(data[i]);
    }

    return oss.str();
}

/**
 * Generic hash function using EVP interface.
 * Returns Result with hex string or error.
 */
inline bishop::rt::Result<std::string> hash_evp(const std::string& data, const EVP_MD* md) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();

    if (!ctx) {
        return bishop::rt::make_error<std::string>("failed to create hash context");
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;

    if (EVP_DigestInit_ex(ctx, md, nullptr) != 1 ||
        EVP_DigestUpdate(ctx, data.c_str(), data.size()) != 1 ||
        EVP_DigestFinal_ex(ctx, hash, &hash_len) != 1) {
        EVP_MD_CTX_free(ctx);
        return bishop::rt::make_error<std::string>("hash computation failed");
    }

    EVP_MD_CTX_free(ctx);
    return bytes_to_hex(hash, hash_len);
}

/**
 * Computes MD5 hash of a string.
 * Returns Result with lowercase hex string or error.
 *
 * WARNING: MD5 is cryptographically broken. Do not use for security purposes
 * such as password hashing or digital signatures. Use SHA256 or stronger.
 */
inline bishop::rt::Result<std::string> md5(const std::string& data) {
    return hash_evp(data, EVP_md5());
}

/**
 * Computes SHA1 hash of a string.
 * Returns Result with lowercase hex string or error.
 *
 * WARNING: SHA1 is cryptographically weak and deprecated for security purposes.
 * Do not use for password hashing or digital signatures. Use SHA256 or stronger.
 */
inline bishop::rt::Result<std::string> sha1(const std::string& data) {
    return hash_evp(data, EVP_sha1());
}

/**
 * Computes SHA256 hash of a string.
 * Returns Result with lowercase hex string or error.
 */
inline bishop::rt::Result<std::string> sha256(const std::string& data) {
    return hash_evp(data, EVP_sha256());
}

/**
 * Computes SHA512 hash of a string.
 * Returns Result with lowercase hex string or error.
 */
inline bishop::rt::Result<std::string> sha512(const std::string& data) {
    return hash_evp(data, EVP_sha512());
}

/**
 * Computes HMAC-SHA256 of data with key.
 * Returns lowercase hex string.
 */
inline std::string hmac_sha256(const std::string& key, const std::string& data) {
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned int result_len = 0;

    HMAC(EVP_sha256(),
         key.c_str(), static_cast<int>(key.size()),
         reinterpret_cast<const unsigned char*>(data.c_str()), data.size(),
         result, &result_len);

    return bytes_to_hex(result, result_len);
}

/**
 * Base64 encoding table.
 */
inline const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * Encodes a string to base64.
 */
inline std::string base64_encode(const std::string& data) {
    if (data.empty()) {
        return "";
    }

    std::string result;
    result.reserve(((data.size() + 2) / 3) * 4);

    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(data.c_str());
    size_t len = data.size();

    for (size_t i = 0; i < len; i += 3) {
        unsigned int triple = bytes[i] << 16;

        if (i + 1 < len) {
            triple |= bytes[i + 1] << 8;
        }

        if (i + 2 < len) {
            triple |= bytes[i + 2];
        }

        result += base64_chars[(triple >> 18) & 0x3F];
        result += base64_chars[(triple >> 12) & 0x3F];
        result += (i + 1 < len) ? base64_chars[(triple >> 6) & 0x3F] : '=';
        result += (i + 2 < len) ? base64_chars[triple & 0x3F] : '=';
    }

    return result;
}

/**
 * Decodes base64 character to value.
 * Returns -1 for invalid characters.
 */
inline int base64_decode_char(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }

    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 26;
    }

    if (c >= '0' && c <= '9') {
        return c - '0' + 52;
    }

    if (c == '+') {
        return 62;
    }

    if (c == '/') {
        return 63;
    }

    return -1;
}

/**
 * Decodes a base64 string.
 * Returns Result with decoded string or error.
 */
inline bishop::rt::Result<std::string> base64_decode(const std::string& data) {
    if (data.empty()) {
        return std::string("");
    }

    if (data.size() % 4 != 0) {
        return bishop::rt::make_error<std::string>("invalid base64 length");
    }

    std::string result;
    result.reserve((data.size() / 4) * 3);

    for (size_t i = 0; i < data.size(); i += 4) {
        int a = base64_decode_char(data[i]);
        int b = base64_decode_char(data[i + 1]);
        int c = (data[i + 2] == '=') ? 0 : base64_decode_char(data[i + 2]);
        int d = (data[i + 3] == '=') ? 0 : base64_decode_char(data[i + 3]);

        if (a < 0 || b < 0 || (data[i + 2] != '=' && c < 0) || (data[i + 3] != '=' && d < 0)) {
            return bishop::rt::make_error<std::string>("invalid base64 character");
        }

        unsigned int triple = (a << 18) | (b << 12) | (c << 6) | d;

        result += static_cast<char>((triple >> 16) & 0xFF);

        if (data[i + 2] != '=') {
            result += static_cast<char>((triple >> 8) & 0xFF);
        }

        if (data[i + 3] != '=') {
            result += static_cast<char>(triple & 0xFF);
        }
    }

    return result;
}

/**
 * Encodes a string to hex.
 */
inline std::string hex_encode(const std::string& data) {
    return bytes_to_hex(reinterpret_cast<const unsigned char*>(data.c_str()), data.size());
}

/**
 * Decodes a hex string.
 * Returns Result with decoded string or error.
 */
inline bishop::rt::Result<std::string> hex_decode(const std::string& data) {
    if (data.empty()) {
        return std::string("");
    }

    if (data.size() % 2 != 0) {
        return bishop::rt::make_error<std::string>("invalid hex length: must be even");
    }

    std::string result;
    result.reserve(data.size() / 2);

    for (size_t i = 0; i < data.size(); i += 2) {
        int high = 0;
        int low = 0;

        if (data[i] >= '0' && data[i] <= '9') {
            high = data[i] - '0';
        } else if (data[i] >= 'a' && data[i] <= 'f') {
            high = data[i] - 'a' + 10;
        } else if (data[i] >= 'A' && data[i] <= 'F') {
            high = data[i] - 'A' + 10;
        } else {
            return bishop::rt::make_error<std::string>("invalid hex character");
        }

        if (data[i + 1] >= '0' && data[i + 1] <= '9') {
            low = data[i + 1] - '0';
        } else if (data[i + 1] >= 'a' && data[i + 1] <= 'f') {
            low = data[i + 1] - 'a' + 10;
        } else if (data[i + 1] >= 'A' && data[i + 1] <= 'F') {
            low = data[i + 1] - 'A' + 10;
        } else {
            return bishop::rt::make_error<std::string>("invalid hex character");
        }

        result += static_cast<char>((high << 4) | low);
    }

    return result;
}

/**
 * Generates a random UUID v4.
 * Format: xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
 * where y is 8, 9, a, or b.
 * Returns Result with UUID string or error.
 */
inline bishop::rt::Result<std::string> uuid() {
    unsigned char bytes[16];

    if (RAND_bytes(bytes, 16) != 1) {
        return bishop::rt::make_error<std::string>("random number generation failed");
    }

    // Set version to 4 (random)
    bytes[6] = (bytes[6] & 0x0F) | 0x40;
    // Set variant to 10xx (RFC 4122)
    bytes[8] = (bytes[8] & 0x3F) | 0x80;

    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    for (int i = 0; i < 16; i++) {
        if (i == 4 || i == 6 || i == 8 || i == 10) {
            oss << '-';
        }

        oss << std::setw(2) << static_cast<int>(bytes[i]);
    }

    return oss.str();
}

/**
 * Generates a UUID v5 (namespace + name based on SHA1).
 * Deterministic: same namespace and name always produce same UUID.
 * Returns Result with UUID string or error.
 */
inline bishop::rt::Result<std::string> uuid_v5(const std::string& ns, const std::string& name) {
    // Combine namespace and name
    std::string combined = ns + name;

    // Hash with SHA1
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();

    if (!ctx) {
        return bishop::rt::make_error<std::string>("failed to create hash context");
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;

    if (EVP_DigestInit_ex(ctx, EVP_sha1(), nullptr) != 1 ||
        EVP_DigestUpdate(ctx, combined.c_str(), combined.size()) != 1 ||
        EVP_DigestFinal_ex(ctx, hash, &hash_len) != 1) {
        EVP_MD_CTX_free(ctx);
        return bishop::rt::make_error<std::string>("hash computation failed");
    }

    EVP_MD_CTX_free(ctx);

    // Set version to 5 (SHA1 name-based)
    hash[6] = (hash[6] & 0x0F) | 0x50;
    // Set variant to 10xx (RFC 4122)
    hash[8] = (hash[8] & 0x3F) | 0x80;

    std::ostringstream oss;
    oss << std::hex << std::setfill('0');

    for (int i = 0; i < 16; i++) {
        if (i == 4 || i == 6 || i == 8 || i == 10) {
            oss << '-';
        }

        oss << std::setw(2) << static_cast<int>(hash[i]);
    }

    return oss.str();
}

/**
 * Generates random bytes.
 * Returns Result with vector of unsigned 8-bit integers or error.
 */
inline bishop::rt::Result<std::vector<uint8_t>> random_bytes(int count) {
    if (count <= 0) {
        return std::vector<uint8_t>();
    }

    std::vector<uint8_t> result(static_cast<size_t>(count));

    if (RAND_bytes(result.data(), count) != 1) {
        return bishop::rt::make_error<std::vector<uint8_t>>("random number generation failed");
    }

    return result;
}

}  // namespace crypto
