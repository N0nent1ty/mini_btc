#include "Mempool.h"
#include "../transaction/Transaction.h"

void Mempool::addTransaction(const Transaction& tx) {
    this->transactions.emplace(tx.id, tx);
}

void Mempool::removeTransaction(const std::string& txid) {
    this->transactions.erase(txid);
}

std::vector<Transaction> Mempool::selectTransactions(size_t maxCount) const {
    std::vector<Transaction> result;
    for (const auto& [id, tx] : transactions) {
        result.push_back(tx);
        if (result.size() >= maxCount) break;
    }
    return result;
}