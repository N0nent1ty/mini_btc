// Blockchain.cpp
#include "Blockchain.h"
#include "../utxomanager/UTXOManager.h"
#include <iostream>

bool Blockchain::addBlock(const Block& block, const UTXOManager& utxos) {
    if (!chain.empty()) {
        const Block& prev = chain.back();

        // 驗證前一個 hash
        if (block.getPreviousHash() != prev.calculateHash()) {
            std::cerr << "[!] Invalid previous hash.\n";
            return false;
        }

        // 驗證難度（預設為3）
        std::string hash = block.calculateHash();
        if (hash.substr(0, 3) != std::string(3, '0')) {
            std::cerr << "[!] Block does not meet difficulty requirements.\n";
            return false;
        }
    }


    // 驗證目前打包中的區塊，所有交易的有效性, 避免打包交易數據時產生多花交易
    if (!block.validateTransactions(utxos)) {
        std::cerr << "[!] Block contains invalid transactions.\n";
        return false;
    }

    chain.push_back(block);
    return true;
}

const Block& Blockchain::getLatestBlock() const {
    return chain.back();
}

bool Blockchain::isValidChain() const {
    for (size_t i = 1; i < chain.size(); ++i) {
        if (chain[i].getPreviousHash() != chain[i - 1].calculateHash()) return false;
        if (chain[i].calculateHash().substr(0, 3) != std::string(3, '0')) return false;
    }
    return true;
}

void Blockchain::print() const {
    for (const auto& block : chain) {
        std::cout << block << "\n";
    }
}
