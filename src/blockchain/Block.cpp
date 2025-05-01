// Block.cpp
#include "../blockchain/Block.h"
#include "../sha256/SHA256.h"
#include "../transaction/Transaction.h"
#include <sstream>

Block::Block(int idx, const std::vector<Transaction>& txs, const std::string& prev)
    : index(idx), transactions(txs), prevHash(prev), nonce(0) {
    timestamp = std::time(nullptr);
    hash = calculateHash();
}

std::string Block::calculateHash() const {
    std::stringstream ss;
    ss << index << timestamp;
    for (const auto& tx : transactions) ss << tx.id;
    ss << prevHash << nonce;
    return sha256(ss.str());
}

void Block::mineBlock(int difficulty) {
    std::string target(difficulty, '0');
    while (hash.substr(0, difficulty) != target) {
        nonce++;
        hash = calculateHash();
    }
}

std::ostream& operator<<(std::ostream& os, const Block& block) {
    os << "Index: " << block.index << "\n"
       << "Timestamp: " << block.timestamp << "\n"
       << "Previous Hash: " << block.prevHash << "\n"
       << "Hash: " << block.hash << "\n"
       << "Nonce: " << block.nonce << "\n";
    os << "Transactions:\n";
    for (const auto& tx : block.transactions) {
        os << " - TxID: " << tx.id << "\n";
    }
    return os;
}


bool Block::validateTransactions(const UTXOManager& utxos) const {
    UTXOManager temp = utxos;
    for (const auto& tx : this->transactions) {
        if (!temp.validateTransaction(tx)) {
            std::cerr << "[!] Invalid transaction in block: " << tx.id << "\n";
            return false;
        }
        temp.addTransaction(tx);
    }
    return true;
}