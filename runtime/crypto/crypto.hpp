/**
 * @file crypto.hpp
 * @brief Bishop cryptographic runtime library.
 *
 * Provides cryptographic utilities for Bishop programs including:
 * - Hashing: MD5, SHA1, SHA256, SHA512 (hex string output)
 * - HMAC: HMAC-SHA256
 * - Encoding: Base64, Hex
 * - UUID: v4 random, v5 namespace-based
 * - Random bytes generation
 *
 * Uses OpenSSL for cryptographic operations.
 */

#pragma once

#include <bishop/std.hpp>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <iomanip>
#include <sstream>
#include <random>
#include <array>

namespace crypto {

/**
 * Converts a byte array to a hex string.
 */
inline std::string bytes_to_hex(const unsigned char* data, size_t len) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    for (size_t i = 0; i < len; i++) {
        ss << std::setw(2) << static_cast<int>(data[i]);
    }

    return ss.str();
}

/**
 * Computes MD5 hash of input string.
 * Returns hex-encoded hash string.
 */
inline std::string md5(const std::string& data) {
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);
    return bytes_to_hex(hash, MD5_DIGEST_LENGTH);
}

/**
 * Computes SHA1 hash of input string.
 * Returns hex-encoded hash string.
 */
inline std::string sha1(const std::string& data) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);
    return bytes_to_hex(hash, SHA_DIGEST_LENGTH);
}

/**
 * Computes SHA256 hash of input string.
 * Returns hex-encoded hash string.
 */
inline std::string sha256(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);
    return bytes_to_hex(hash, SHA256_DIGEST_LENGTH);
}

/**
 * Computes SHA512 hash of input string.
 * Returns hex-encoded hash string.
 */
inline std::string sha512(const std::string& data) {
    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);
    return bytes_to_hex(hash, SHA512_DIGEST_LENGTH);
}

/**
 * Computes HMAC-SHA256 of data with given key.
 * Returns hex-encoded HMAC string.
 */
inline std::string hmac_sha256(const std::string& key, const std::string& data) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    HMAC(EVP_sha256(),
         key.c_str(), static_cast<int>(key.size()),
         reinterpret_cast<const unsigned char*>(data.c_str()), data.size(),
         hash, &hash_len);

    return bytes_to_hex(hash, hash_len);
}

/**
 * Base64 encoding alphabet.
 */
static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

/**
 * Encodes a string to Base64.
 */
inline std::string base64_encode(const std::string& data) {
    std::string result;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    size_t in_len = data.size();
    const unsigned char* bytes_to_encode = reinterpret_cast<const unsigned char*>(data.c_str());

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);

        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; i < 4; i++) {
                result += base64_chars[char_array_4[i]];
            }

            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; j < i + 1; j++) {
            result += base64_chars[char_array_4[j]];
        }

        while (i++ < 3) {
            result += '=';
        }
    }

    return result;
}

/**
 * Finds position of a character in base64 alphabet.
 * Returns -1 if not found.
 */
inline int base64_char_value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

/**
 * Decodes a Base64 string.
 * Returns a Result containing the decoded string or an error.
 */
inline bishop::rt::Result<std::string> base64_decode(const std::string& encoded) {
    if (encoded.empty()) {
        return std::string("");
    }

    size_t in_len = encoded.size();

    if (in_len % 4 != 0) {
        return bishop::rt::make_error("invalid base64: length not multiple of 4");
    }

    size_t out_len = in_len / 4 * 3;

    if (encoded[in_len - 1] == '=') out_len--;
    if (encoded[in_len - 2] == '=') out_len--;

    std::string result;
    result.reserve(out_len);

    int i = 0;
    int j = 0;
    unsigned char char_array_4[4];
    unsigned char char_array_3[3];

    for (size_t pos = 0; pos < in_len; pos++) {
        char c = encoded[pos];

        if (c == '=') {
            break;
        }

        int val = base64_char_value(c);

        if (val < 0) {
            return bishop::rt::make_error("invalid base64: illegal character");
        }

        char_array_4[i++] = static_cast<unsigned char>(val);

        if (i == 4) {
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; i < 3; i++) {
                result += static_cast<char>(char_array_3[i]);
            }

            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++) {
            char_array_4[j] = 0;
        }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; j < i - 1; j++) {
            result += static_cast<char>(char_array_3[j]);
        }
    }

    return result;
}

/**
 * Encodes a string to hexadecimal.
 */
inline std::string hex_encode(const std::string& data) {
    return bytes_to_hex(reinterpret_cast<const unsigned char*>(data.c_str()), data.size());
}

/**
 * Converts a hex character to its value.
 * Returns -1 if invalid.
 */
inline int hex_char_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

/**
 * Decodes a hexadecimal string.
 * Returns a Result containing the decoded string or an error.
 */
inline bishop::rt::Result<std::string> hex_decode(const std::string& hex) {
    if (hex.empty()) {
        return std::string("");
    }

    if (hex.size() % 2 != 0) {
        return bishop::rt::make_error("invalid hex: odd length");
    }

    std::string result;
    result.reserve(hex.size() / 2);

    for (size_t i = 0; i < hex.size(); i += 2) {
        int high = hex_char_value(hex[i]);
        int low = hex_char_value(hex[i + 1]);

        if (high < 0 || low < 0) {
            return bishop::rt::make_error("invalid hex: illegal character");
        }

        result += static_cast<char>((high << 4) | low);
    }

    return result;
}

/**
 * Generates a random UUID v4.
 * Format: xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
 * where x is random hex and y is one of 8, 9, a, b.
 */
inline std::string uuid() {
    unsigned char bytes[16];

    if (RAND_bytes(bytes, 16) != 1) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        for (int i = 0; i < 16; i++) {
            bytes[i] = static_cast<unsigned char>(dis(gen));
        }
    }

    bytes[6] = (bytes[6] & 0x0f) | 0x40;
    bytes[8] = (bytes[8] & 0x3f) | 0x80;

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    ss << std::setw(2) << static_cast<int>(bytes[0]);
    ss << std::setw(2) << static_cast<int>(bytes[1]);
    ss << std::setw(2) << static_cast<int>(bytes[2]);
    ss << std::setw(2) << static_cast<int>(bytes[3]);
    ss << '-';
    ss << std::setw(2) << static_cast<int>(bytes[4]);
    ss << std::setw(2) << static_cast<int>(bytes[5]);
    ss << '-';
    ss << std::setw(2) << static_cast<int>(bytes[6]);
    ss << std::setw(2) << static_cast<int>(bytes[7]);
    ss << '-';
    ss << std::setw(2) << static_cast<int>(bytes[8]);
    ss << std::setw(2) << static_cast<int>(bytes[9]);
    ss << '-';
    ss << std::setw(2) << static_cast<int>(bytes[10]);
    ss << std::setw(2) << static_cast<int>(bytes[11]);
    ss << std::setw(2) << static_cast<int>(bytes[12]);
    ss << std::setw(2) << static_cast<int>(bytes[13]);
    ss << std::setw(2) << static_cast<int>(bytes[14]);
    ss << std::setw(2) << static_cast<int>(bytes[15]);

    return ss.str();
}

/**
 * Generates a UUID v5 based on namespace and name.
 * Uses SHA1 hash of namespace UUID + name.
 */
inline std::string uuid_v5(const std::string& ns, const std::string& name) {
    std::string namespace_uuid = ns;

    if (ns == "dns" || ns == "DNS") {
        namespace_uuid = "6ba7b810-9dad-11d1-80b4-00c04fd430c8";
    } else if (ns == "url" || ns == "URL") {
        namespace_uuid = "6ba7b811-9dad-11d1-80b4-00c04fd430c8";
    } else if (ns == "oid" || ns == "OID") {
        namespace_uuid = "6ba7b812-9dad-11d1-80b4-00c04fd430c8";
    } else if (ns == "x500" || ns == "X500") {
        namespace_uuid = "6ba7b814-9dad-11d1-80b4-00c04fd430c8";
    }

    unsigned char ns_bytes[16];
    int byte_idx = 0;

    for (size_t i = 0; i < namespace_uuid.size() && byte_idx < 16; i++) {
        if (namespace_uuid[i] == '-') continue;

        int high = hex_char_value(namespace_uuid[i]);
        int low = hex_char_value(namespace_uuid[i + 1]);

        if (high >= 0 && low >= 0) {
            ns_bytes[byte_idx++] = static_cast<unsigned char>((high << 4) | low);
            i++;
        }
    }

    std::string to_hash;
    to_hash.append(reinterpret_cast<char*>(ns_bytes), 16);
    to_hash.append(name);

    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(to_hash.c_str()), to_hash.size(), hash);

    hash[6] = (hash[6] & 0x0f) | 0x50;
    hash[8] = (hash[8] & 0x3f) | 0x80;

    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    ss << std::setw(2) << static_cast<int>(hash[0]);
    ss << std::setw(2) << static_cast<int>(hash[1]);
    ss << std::setw(2) << static_cast<int>(hash[2]);
    ss << std::setw(2) << static_cast<int>(hash[3]);
    ss << '-';
    ss << std::setw(2) << static_cast<int>(hash[4]);
    ss << std::setw(2) << static_cast<int>(hash[5]);
    ss << '-';
    ss << std::setw(2) << static_cast<int>(hash[6]);
    ss << std::setw(2) << static_cast<int>(hash[7]);
    ss << '-';
    ss << std::setw(2) << static_cast<int>(hash[8]);
    ss << std::setw(2) << static_cast<int>(hash[9]);
    ss << '-';
    ss << std::setw(2) << static_cast<int>(hash[10]);
    ss << std::setw(2) << static_cast<int>(hash[11]);
    ss << std::setw(2) << static_cast<int>(hash[12]);
    ss << std::setw(2) << static_cast<int>(hash[13]);
    ss << std::setw(2) << static_cast<int>(hash[14]);
    ss << std::setw(2) << static_cast<int>(hash[15]);

    return ss.str();
}

/**
 * Generates cryptographically secure random bytes.
 * Returns a list of unsigned 8-bit integers.
 */
inline std::vector<int> random_bytes(int count) {
    std::vector<int> result;

    if (count <= 0) {
        return result;
    }

    result.reserve(count);
    std::vector<unsigned char> bytes(count);

    if (RAND_bytes(bytes.data(), count) != 1) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);

        for (int i = 0; i < count; i++) {
            bytes[i] = static_cast<unsigned char>(dis(gen));
        }
    }

    for (int i = 0; i < count; i++) {
        result.push_back(static_cast<int>(bytes[i]));
    }

    return result;
}

}  // namespace crypto
