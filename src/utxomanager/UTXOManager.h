// UTXOManager.h
// Unspent Transaction Output Manager
#pragma once
#include "../transaction/Transaction.h"
#include <map>
#include <string>
#include <optional>



class UTXOManager {
public:
    void addTransaction(const Transaction& tx);
    bool isUnspent(const TxIn& in) const;
    void consumeInput(const TxIn& in);
    double getBalance(const std::string& address) const;
    bool validateTransaction(const Transaction& tx) const;
    std::optional<TxOut> getUTXO(const std::string& txid, int index) const;
private:
    // key: txid + outputIndex, value: TxOut
    std::map<std::string, TxOut> utxos;
};