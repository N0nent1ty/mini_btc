// UTXOManager.cpp
#include "UTXOManager.h"

std::string toKey(const std::string& txid, int index) {
    return txid + ":" + std::to_string(index);
}

void UTXOManager::addTransaction(const Transaction& tx) {
    for (size_t i = 0; i < tx.outputs.size(); ++i) {
        std::string key = toKey(tx.id, static_cast<int>(i));
        utxos[key] = tx.outputs[i];
    }
    for (const auto& in : tx.inputs) {
        consumeInput(in);
    }
}

bool UTXOManager::isUnspent(const TxIn& in) const {
    return utxos.find(toKey(in.prevTxId, in.outputIndex)) != utxos.end();
}

void UTXOManager::consumeInput(const TxIn& in) {
    utxos.erase(toKey(in.prevTxId, in.outputIndex));
}

double UTXOManager::getBalance(const std::string& address) const {
    double balance = 0.0;
    for (const auto& [_, txout] : utxos) {
        if (txout.recipient == address) {
            balance += txout.amount;
        }
    }
    return balance;
}
