// Transaction.cpp
#include "Transaction.h"
#include "../sha256/SHA256.h"
#include <sstream>

Transaction::Transaction(const std::vector<TxIn>& in, const std::vector<TxOut>& out)
    : inputs(in), outputs(out) {
    id = calculateHash();
}

std::string Transaction::calculateHash() const {
    std::stringstream ss;
    for (const auto& in : inputs) {
        ss << in.prevTxId << in.outputIndex;
    }
    for (const auto& out : outputs) {
        ss << out.recipient << out.amount;
    }
    return sha256(ss.str());
}
