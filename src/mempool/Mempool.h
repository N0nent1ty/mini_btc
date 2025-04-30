#pragma once
#include "../transaction/Transaction.h"
#include <unordered_map>

class Mempool {
public:
    void addTransaction(const Transaction& tx);
    void removeTransaction(const std::string& txid);
    std::vector<Transaction> selectTransactions(size_t maxCount) const;

private:
    std::unordered_map<std::string, Transaction> transactions;
};