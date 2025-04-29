#include "crypto.h"
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/sha.h>
#include <openssl/err.h>
#include <sstream>
#include <iomanip>
#include <vector>

namespace {

// Utility: Convert hex string to byte array
std::vector<unsigned char> hexToBytes(const std::string& hex) {
    std::vector<unsigned char> bytes;
    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        unsigned char byte = (unsigned char) strtol(byteString.c_str(), nullptr, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

// Utility: Convert byte array to hex string
std::string bytesToHex(const unsigned char* data, size_t len) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < len; ++i) {
        ss << std::setw(2) << static_cast<int>(data[i]);
    }
    return ss.str();
}

} // namespace

std::string signMessage(const std::string& privateKeyHex, const std::string& messageHash) {
    EC_KEY* ecKey = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!ecKey) throw std::runtime_error("Failed to create EC Key");

    // Load private key hex into BIGNUM
    BIGNUM* privKeyBN = BN_new();
    BN_hex2bn(&privKeyBN, privateKeyHex.c_str());
    if (!EC_KEY_set_private_key(ecKey, privKeyBN))
        throw std::runtime_error("Failed to set private key");

    // Prepare hash
    auto hashBytes = hexToBytes(messageHash);

    // Sign the message hash
    ECDSA_SIG* sig = ECDSA_do_sign(hashBytes.data(), hashBytes.size(), ecKey);
    if (!sig) throw std::runtime_error("Failed to sign message");

    // Encode signature
    unsigned char* der = nullptr;
    int derLen = i2d_ECDSA_SIG(sig, &der);
    std::string signatureHex = bytesToHex(der, derLen);

    // Clean up
    ECDSA_SIG_free(sig);
    EC_KEY_free(ecKey);
    BN_free(privKeyBN);
    OPENSSL_free(der);

    return signatureHex;
}

bool verifySignature(const std::string& publicKeyHex, const std::string& messageHash, const std::string& signatureHex) {
    EC_KEY* ecKey = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!ecKey) throw std::runtime_error("Failed to create EC Key");

    auto pubKeyBytes = hexToBytes(publicKeyHex);

    // Set public key
    const unsigned char* pubKeyPtr = pubKeyBytes.data();
    o2i_ECPublicKey(&ecKey, &pubKeyPtr, pubKeyBytes.size());

    auto hashBytes = hexToBytes(messageHash);
    auto sigBytes = hexToBytes(signatureHex);

    // Decode signature
    const unsigned char* sigPtr = sigBytes.data();
    ECDSA_SIG* sig = d2i_ECDSA_SIG(nullptr, &sigPtr, sigBytes.size());
    if (!sig) {
        EC_KEY_free(ecKey);
        return false;
    }

    bool valid = ECDSA_do_verify(hashBytes.data(), hashBytes.size(), sig, ecKey) == 1;

    ECDSA_SIG_free(sig);
    EC_KEY_free(ecKey);

    return valid;
}


std::pair<std::string, std::string> generateKeyPair() {
    EC_KEY* ecKey = EC_KEY_new_by_curve_name(NID_secp256k1);
    if (!ecKey) throw std::runtime_error("Failed to create EC Key");

    if (!EC_KEY_generate_key(ecKey))
        throw std::runtime_error("Failed to generate key pair");

    // Retrieve the private key
    const BIGNUM* privKeyBN = EC_KEY_get0_private_key(ecKey);
    char* privKeyHex = BN_bn2hex(privKeyBN);

    // Retrieve the public key (compressed or uncompressed format)
    int pubKeyLen = i2o_ECPublicKey(ecKey, nullptr);
    std::vector<unsigned char> pubKeyBytes(pubKeyLen);
    unsigned char* pubKeyPtr = pubKeyBytes.data();
    i2o_ECPublicKey(ecKey, &pubKeyPtr);

    std::string privKeyStr(privKeyHex);
    std::string pubKeyStr = bytesToHex(pubKeyBytes.data(), pubKeyBytes.size());

    // Clean up
    EC_KEY_free(ecKey);
    OPENSSL_free(privKeyHex);

    return {privKeyStr, pubKeyStr};
}