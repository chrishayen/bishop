/**
 * @file crypto.cpp
 * @brief Built-in crypto module implementation.
 *
 * Creates the AST definitions for the crypto module.
 * The actual runtime is in runtime/crypto/crypto.hpp and linked with OpenSSL.
 */

/**
 * @nog_fn md5
 * @module crypto
 * @description Computes MD5 hash of a string.
 * WARNING: MD5 is cryptographically broken. Do not use for security purposes
 * such as password hashing or digital signatures. Use SHA256 or stronger.
 * @param data str - Data to hash
 * @returns str or err - Lowercase hex string of the hash, or error
 * @example
 * import crypto;
 * hash := crypto.md5("hello") or return;
 * // hash == "5d41402abc4b2a76b9719d911017c592"
 */

/**
 * @nog_fn sha1
 * @module crypto
 * @description Computes SHA1 hash of a string.
 * WARNING: SHA1 is cryptographically weak and deprecated for security purposes.
 * Do not use for password hashing or digital signatures. Use SHA256 or stronger.
 * @param data str - Data to hash
 * @returns str or err - Lowercase hex string of the hash, or error
 * @example
 * import crypto;
 * hash := crypto.sha1("hello") or return;
 */

/**
 * @nog_fn sha256
 * @module crypto
 * @description Computes SHA256 hash of a string.
 * @param data str - Data to hash
 * @returns str or err - Lowercase hex string of the hash, or error
 * @example
 * import crypto;
 * hash := crypto.sha256("hello") or return;
 */

/**
 * @nog_fn sha512
 * @module crypto
 * @description Computes SHA512 hash of a string.
 * @param data str - Data to hash
 * @returns str or err - Lowercase hex string of the hash, or error
 * @example
 * import crypto;
 * hash := crypto.sha512("hello") or return;
 */

/**
 * @nog_fn hmac_sha256
 * @module crypto
 * @description Computes HMAC-SHA256 of data with key.
 * @param key str - Secret key
 * @param data str - Data to authenticate
 * @returns str - Lowercase hex string of the HMAC
 * @example
 * import crypto;
 * hmac := crypto.hmac_sha256("secret", "message");
 */

/**
 * @nog_fn base64_encode
 * @module crypto
 * @description Encodes a string to base64.
 * @param data str - Data to encode
 * @returns str - Base64 encoded string
 * @example
 * import crypto;
 * encoded := crypto.base64_encode("Hello!");
 * // encoded == "SGVsbG8h"
 */

/**
 * @nog_fn base64_decode
 * @module crypto
 * @description Decodes a base64 string.
 * @param data str - Base64 encoded string
 * @returns str or err - Decoded string or error if invalid
 * @example
 * import crypto;
 * decoded := crypto.base64_decode("SGVsbG8h") or return;
 */

/**
 * @nog_fn hex_encode
 * @module crypto
 * @description Encodes a string to hex.
 * @param data str - Data to encode
 * @returns str - Lowercase hex string
 * @example
 * import crypto;
 * hex := crypto.hex_encode("hello");
 * // hex == "68656c6c6f"
 */

/**
 * @nog_fn hex_decode
 * @module crypto
 * @description Decodes a hex string.
 * @param data str - Hex encoded string
 * @returns str or err - Decoded string or error if invalid
 * @example
 * import crypto;
 * decoded := crypto.hex_decode("68656c6c6f") or return;
 */

/**
 * @nog_fn uuid
 * @module crypto
 * @description Generates a random UUID v4.
 * @returns str or err - UUID in format xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx, or error
 * @example
 * import crypto;
 * id := crypto.uuid() or return;
 */

/**
 * @nog_fn uuid_v5
 * @module crypto
 * @description Generates a deterministic UUID v5 from namespace and name.
 * @param namespace str - Namespace string
 * @param name str - Name string
 * @returns str or err - UUID in format xxxxxxxx-xxxx-5xxx-yxxx-xxxxxxxxxxxx, or error
 * @example
 * import crypto;
 * id := crypto.uuid_v5("myapp", "user123") or return;
 */

/**
 * @nog_fn random_bytes
 * @module crypto
 * @description Generates cryptographically secure random bytes.
 * @param count int - Number of bytes to generate
 * @returns List<u8> or err - List of random bytes, or error
 * @example
 * import crypto;
 * bytes := crypto.random_bytes(32) or return;
 */

#include "crypto.hpp"

using namespace std;

namespace nog::stdlib {

/**
 * Creates the AST for the built-in crypto module.
 */
unique_ptr<Program> create_crypto_module() {
    auto program = make_unique<Program>();

    // fn md5(str data) -> str or err
    auto md5_fn = make_unique<FunctionDef>();
    md5_fn->name = "md5";
    md5_fn->visibility = Visibility::Public;
    md5_fn->params.push_back({"str", "data"});
    md5_fn->return_type = "str";
    md5_fn->error_type = "err";
    program->functions.push_back(move(md5_fn));

    // fn sha1(str data) -> str or err
    auto sha1_fn = make_unique<FunctionDef>();
    sha1_fn->name = "sha1";
    sha1_fn->visibility = Visibility::Public;
    sha1_fn->params.push_back({"str", "data"});
    sha1_fn->return_type = "str";
    sha1_fn->error_type = "err";
    program->functions.push_back(move(sha1_fn));

    // fn sha256(str data) -> str or err
    auto sha256_fn = make_unique<FunctionDef>();
    sha256_fn->name = "sha256";
    sha256_fn->visibility = Visibility::Public;
    sha256_fn->params.push_back({"str", "data"});
    sha256_fn->return_type = "str";
    sha256_fn->error_type = "err";
    program->functions.push_back(move(sha256_fn));

    // fn sha512(str data) -> str or err
    auto sha512_fn = make_unique<FunctionDef>();
    sha512_fn->name = "sha512";
    sha512_fn->visibility = Visibility::Public;
    sha512_fn->params.push_back({"str", "data"});
    sha512_fn->return_type = "str";
    sha512_fn->error_type = "err";
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
    auto b64_enc_fn = make_unique<FunctionDef>();
    b64_enc_fn->name = "base64_encode";
    b64_enc_fn->visibility = Visibility::Public;
    b64_enc_fn->params.push_back({"str", "data"});
    b64_enc_fn->return_type = "str";
    program->functions.push_back(move(b64_enc_fn));

    // fn base64_decode(str data) -> str or err
    auto b64_dec_fn = make_unique<FunctionDef>();
    b64_dec_fn->name = "base64_decode";
    b64_dec_fn->visibility = Visibility::Public;
    b64_dec_fn->params.push_back({"str", "data"});
    b64_dec_fn->return_type = "str";
    b64_dec_fn->error_type = "err";
    program->functions.push_back(move(b64_dec_fn));

    // fn hex_encode(str data) -> str
    auto hex_enc_fn = make_unique<FunctionDef>();
    hex_enc_fn->name = "hex_encode";
    hex_enc_fn->visibility = Visibility::Public;
    hex_enc_fn->params.push_back({"str", "data"});
    hex_enc_fn->return_type = "str";
    program->functions.push_back(move(hex_enc_fn));

    // fn hex_decode(str data) -> str or err
    auto hex_dec_fn = make_unique<FunctionDef>();
    hex_dec_fn->name = "hex_decode";
    hex_dec_fn->visibility = Visibility::Public;
    hex_dec_fn->params.push_back({"str", "data"});
    hex_dec_fn->return_type = "str";
    hex_dec_fn->error_type = "err";
    program->functions.push_back(move(hex_dec_fn));

    // fn uuid() -> str or err
    auto uuid_fn = make_unique<FunctionDef>();
    uuid_fn->name = "uuid";
    uuid_fn->visibility = Visibility::Public;
    uuid_fn->return_type = "str";
    uuid_fn->error_type = "err";
    program->functions.push_back(move(uuid_fn));

    // fn uuid_v5(str namespace, str name) -> str or err
    auto uuid_v5_fn = make_unique<FunctionDef>();
    uuid_v5_fn->name = "uuid_v5";
    uuid_v5_fn->visibility = Visibility::Public;
    uuid_v5_fn->params.push_back({"str", "namespace"});
    uuid_v5_fn->params.push_back({"str", "name"});
    uuid_v5_fn->return_type = "str";
    uuid_v5_fn->error_type = "err";
    program->functions.push_back(move(uuid_v5_fn));

    // fn random_bytes(int count) -> List<u8> or err
    auto random_bytes_fn = make_unique<FunctionDef>();
    random_bytes_fn->name = "random_bytes";
    random_bytes_fn->visibility = Visibility::Public;
    random_bytes_fn->params.push_back({"int", "count"});
    random_bytes_fn->return_type = "List<u8>";
    random_bytes_fn->error_type = "err";
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
