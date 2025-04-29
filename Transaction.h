// Transaction.h
#pragma once
#include <string>
#include <vector>

struct TxIn {
    std::string prevTxId;
    int outputIndex;
};

struct TxOut {
    std::string recipient;
    double amount;
};

class Transaction {
public:
    std::string id;
    std::vector<TxIn> inputs;
    std::vector<TxOut> outputs;

    Transaction(const std::vector<TxIn>& in, const std::vector<TxOut>& out);
    std::string calculateHash() const;
};