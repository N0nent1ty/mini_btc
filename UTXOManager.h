// UTXOManager.h
// Unspent Transaction Output Manager
#pragma once
#include "Transaction.h"
#include <map>
#include <string>

class UTXOManager {
public:
    void addTransaction(const Transaction& tx);
    bool isUnspent(const TxIn& in) const;
    void consumeInput(const TxIn& in);
    double getBalance(const std::string& address) const;

private:
    // key: txid + outputIndex, value: TxOut
    std::map<std::string, TxOut> utxos;
};