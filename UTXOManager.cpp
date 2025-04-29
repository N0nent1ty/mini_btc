// UTXOManager.cpp
#include "UTXOManager.h"
#include "crypto.h"
#include <iostream>

std::string toKey(const std::string& txid, int index) {
    return txid + ":" + std::to_string(index);
}

bool UTXOManager::validateTransaction(const Transaction& tx) const {
    std::string messageHash = tx.calculateHash();

    for (const auto& input : tx.inputs) {
        // 用 txid + outputIndex 作為 key 查找
        std::string key = toKey(input.prevTxId, input.outputIndex);
        auto it = utxos.find(key);

        if (it == utxos.end()) {
            std::cerr << "[!] UTXO not found for input key: " << key << "\n";
            return false;
        }

        const auto& utxo = it->second;

        // 檢查該UTXO的recipient是否為提供此input的公鑰
        if (utxo.recipient != input.publicKey) {
            std::cerr << "[!] Public key does not match UTXO owner\n";
            return false;
        }

        // 驗證簽章
        if (!verifySignature(input.publicKey, messageHash, input.signature)) {
            std::cerr << "[!] Signature verification failed\n";
            return false;
        }
    }

    return true; // 所有 input 都合法，交易有效
}


void UTXOManager::addTransaction(const Transaction &tx)
{
    for (size_t i = 0; i < tx.outputs.size(); ++i)
    {
        std::string key = toKey(tx.id, static_cast<int>(i));
        utxos[key] = tx.outputs[i];
    }
    for (const auto &in : tx.inputs)
    {
        consumeInput(in);
    }
}

/*
Here is the translation of the comments into English:

1. Check if the UTXO (Unspent Transaction Output) for the `TxIn` exists.
2. Retrieve the recipient corresponding to the UTXO.
3. Verify whether the public key included in the `TxIn` matches this recipient address.
4. Verify whether the signature made by the public key is valid for this transaction.
*/
bool UTXOManager::isUnspent(const TxIn &in) const
{
    return utxos.find(toKey(in.prevTxId, in.outputIndex)) != utxos.end();
}

void UTXOManager::consumeInput(const TxIn &in)
{
    utxos.erase(toKey(in.prevTxId, in.outputIndex));
}

double UTXOManager::getBalance(const std::string &address) const
{
    double balance = 0.0;
    for (const auto &[_, txout] : utxos)
    {
        if (txout.recipient == address)
        {
            balance += txout.amount;
        }
    }
    return balance;
}
