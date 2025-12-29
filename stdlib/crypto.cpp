/**
 * @file crypto.cpp
 * @brief Built-in crypto module implementation.
 *
 * Creates the AST definitions for the crypto module.
 * The actual runtime is in runtime/crypto/crypto.hpp.
 */

/**
 * @nog_fn md5
 * @module crypto
 * @description Computes MD5 hash of input string.
 * @param data str - The data to hash
 * @returns str - Hex-encoded MD5 hash (32 characters)
 * @example
 * import crypto;
 * hash := crypto.md5("Hello, World!");
 * print(hash);  // "65a8e27d8879283831b664bd8b7f0ad4"
 */

/**
 * @nog_fn sha1
 * @module crypto
 * @description Computes SHA1 hash of input string.
 * @param data str - The data to hash
 * @returns str - Hex-encoded SHA1 hash (40 characters)
 * @example
 * import crypto;
 * hash := crypto.sha1("Hello, World!");
 * print(hash);  // "0a0a9f2a6772942557ab5355d76af442f8f65e01"
 */

/**
 * @nog_fn sha256
 * @module crypto
 * @description Computes SHA256 hash of input string.
 * @param data str - The data to hash
 * @returns str - Hex-encoded SHA256 hash (64 characters)
 * @example
 * import crypto;
 * hash := crypto.sha256("Hello, World!");
 * print(hash);  // "dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f"
 */

/**
 * @nog_fn sha512
 * @module crypto
 * @description Computes SHA512 hash of input string.
 * @param data str - The data to hash
 * @returns str - Hex-encoded SHA512 hash (128 characters)
 * @example
 * import crypto;
 * hash := crypto.sha512("Hello, World!");
 */

/**
 * @nog_fn hmac_sha256
 * @module crypto
 * @description Computes HMAC-SHA256 of data with given key.
 * @param key str - The secret key
 * @param data str - The data to authenticate
 * @returns str - Hex-encoded HMAC (64 characters)
 * @example
 * import crypto;
 * mac := crypto.hmac_sha256("secret", "message");
 */

/**
 * @nog_fn base64_encode
 * @module crypto
 * @description Encodes a string to Base64.
 * @param data str - The data to encode
 * @returns str - Base64-encoded string
 * @example
 * import crypto;
 * encoded := crypto.base64_encode("Hello, World!");
 * print(encoded);  // "SGVsbG8sIFdvcmxkIQ=="
 */

/**
 * @nog_fn base64_decode
 * @module crypto
 * @description Decodes a Base64 string.
 * @param data str - The Base64-encoded string
 * @returns str or err - Decoded string or error if invalid
 * @example
 * import crypto;
 * decoded := crypto.base64_decode("SGVsbG8sIFdvcmxkIQ==") or {
 *     print("decode error");
 *     return;
 * };
 */

/**
 * @nog_fn hex_encode
 * @module crypto
 * @description Encodes a string to hexadecimal.
 * @param data str - The data to encode
 * @returns str - Hex-encoded string
 * @example
 * import crypto;
 * hex := crypto.hex_encode("Hi");
 * print(hex);  // "4869"
 */

/**
 * @nog_fn hex_decode
 * @module crypto
 * @description Decodes a hexadecimal string.
 * @param data str - The hex-encoded string
 * @returns str or err - Decoded string or error if invalid
 * @example
 * import crypto;
 * decoded := crypto.hex_decode("4869") or return;
 * print(decoded);  // "Hi"
 */

/**
 * @nog_fn uuid
 * @module crypto
 * @description Generates a random UUID v4.
 * @returns str - UUID in format xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
 * @example
 * import crypto;
 * id := crypto.uuid();
 * print(id);  // "550e8400-e29b-41d4-a716-446655440000"
 */

/**
 * @nog_fn uuid_v5
 * @module crypto
 * @description Generates a UUID v5 based on namespace and name.
 * @param namespace str - Namespace UUID or predefined name (dns, url, oid, x500)
 * @param name str - The name to generate UUID for
 * @returns str - Deterministic UUID v5
 * @example
 * import crypto;
 * id := crypto.uuid_v5("dns", "example.com");
 */

/**
 * @nog_fn random_bytes
 * @module crypto
 * @description Generates cryptographically secure random bytes.
 * @param count int - Number of bytes to generate
 * @returns List<int> - List of random byte values (0-255)
 * @example
 * import crypto;
 * bytes := crypto.random_bytes(32);  // 32 random bytes for a key
 */

#include "crypto.hpp"

using namespace std;

namespace nog::stdlib {

/**
 * Creates the AST for the built-in crypto module.
 */
unique_ptr<Program> create_crypto_module() {
    auto program = make_unique<Program>();

    // fn md5(str data) -> str
    auto md5_fn = make_unique<FunctionDef>();
    md5_fn->name = "md5";
    md5_fn->visibility = Visibility::Public;
    md5_fn->params.push_back({"str", "data"});
    md5_fn->return_type = "str";
    program->functions.push_back(move(md5_fn));

    // fn sha1(str data) -> str
    auto sha1_fn = make_unique<FunctionDef>();
    sha1_fn->name = "sha1";
    sha1_fn->visibility = Visibility::Public;
    sha1_fn->params.push_back({"str", "data"});
    sha1_fn->return_type = "str";
    program->functions.push_back(move(sha1_fn));

    // fn sha256(str data) -> str
    auto sha256_fn = make_unique<FunctionDef>();
    sha256_fn->name = "sha256";
    sha256_fn->visibility = Visibility::Public;
    sha256_fn->params.push_back({"str", "data"});
    sha256_fn->return_type = "str";
    program->functions.push_back(move(sha256_fn));

    // fn sha512(str data) -> str
    auto sha512_fn = make_unique<FunctionDef>();
    sha512_fn->name = "sha512";
    sha512_fn->visibility = Visibility::Public;
    sha512_fn->params.push_back({"str", "data"});
    sha512_fn->return_type = "str";
    program->functions.push_back(move(sha512_fn));

    // fn hmac_sha256(str key, str data) -> str
    auto hmac_fn = make_unique<FunctionDef>();
    hmac_fn->name = "hmac_sha256";
    hmac_fn->visibility = Visibility::Public;
    hmac_fn->params.push_back({"str", "key"});
    hmac_fn->params.push_back({"str", "data"});
    hmac_fn->return_type = "str";
    program->functions.push_back(move(hmac_fn));

    // fn base64_encode(str data) -> str
    auto b64_encode_fn = make_unique<FunctionDef>();
    b64_encode_fn->name = "base64_encode";
    b64_encode_fn->visibility = Visibility::Public;
    b64_encode_fn->params.push_back({"str", "data"});
    b64_encode_fn->return_type = "str";
    program->functions.push_back(move(b64_encode_fn));

    // fn base64_decode(str data) -> str or err
    auto b64_decode_fn = make_unique<FunctionDef>();
    b64_decode_fn->name = "base64_decode";
    b64_decode_fn->visibility = Visibility::Public;
    b64_decode_fn->params.push_back({"str", "data"});
    b64_decode_fn->return_type = "str";
    b64_decode_fn->is_fallible = true;
    program->functions.push_back(move(b64_decode_fn));

    // fn hex_encode(str data) -> str
    auto hex_encode_fn = make_unique<FunctionDef>();
    hex_encode_fn->name = "hex_encode";
    hex_encode_fn->visibility = Visibility::Public;
    hex_encode_fn->params.push_back({"str", "data"});
    hex_encode_fn->return_type = "str";
    program->functions.push_back(move(hex_encode_fn));

    // fn hex_decode(str data) -> str or err
    auto hex_decode_fn = make_unique<FunctionDef>();
    hex_decode_fn->name = "hex_decode";
    hex_decode_fn->visibility = Visibility::Public;
    hex_decode_fn->params.push_back({"str", "data"});
    hex_decode_fn->return_type = "str";
    hex_decode_fn->is_fallible = true;
    program->functions.push_back(move(hex_decode_fn));

    // fn uuid() -> str
    auto uuid_fn = make_unique<FunctionDef>();
    uuid_fn->name = "uuid";
    uuid_fn->visibility = Visibility::Public;
    uuid_fn->return_type = "str";
    program->functions.push_back(move(uuid_fn));

    // fn uuid_v5(str namespace, str name) -> str
    auto uuid_v5_fn = make_unique<FunctionDef>();
    uuid_v5_fn->name = "uuid_v5";
    uuid_v5_fn->visibility = Visibility::Public;
    uuid_v5_fn->params.push_back({"str", "ns"});
    uuid_v5_fn->params.push_back({"str", "name"});
    uuid_v5_fn->return_type = "str";
    program->functions.push_back(move(uuid_v5_fn));

    // fn random_bytes(int count) -> List<int>
    auto random_bytes_fn = make_unique<FunctionDef>();
    random_bytes_fn->name = "random_bytes";
    random_bytes_fn->visibility = Visibility::Public;
    random_bytes_fn->params.push_back({"int", "count"});
    random_bytes_fn->return_type = "List<int>";
    program->functions.push_back(move(random_bytes_fn));

    return program;
}

/**
 * Returns empty - crypto.hpp is included at the top of generated code
 * for precompiled header support.
 */
string generate_crypto_runtime() {
    return "";
}

}  // namespace nog::stdlib
