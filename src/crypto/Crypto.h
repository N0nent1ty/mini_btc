#pragma once

#include <string>


std::pair<std::string, std::string> generateKeyPair();
std::string signMessage(const std::string& privateKeyHex, const std::string& messageHash);
bool verifySignature(const std::string& publicKeyHex, const std::string& messageHash, const std::string& signatureHex);