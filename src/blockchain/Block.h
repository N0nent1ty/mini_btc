// Block.h
#pragma once
#include "../transaction/Transaction.h"
#include "../utxomanager/UTXOManager.h"
#include <string>
#include <vector>
#include <ctime>
#include <iostream>

class Block {
public:
    Block(int index, const std::vector<Transaction>& transactions, const std::string& prevHash);

    void mineBlock(int difficulty);
    std::string calculateHash() const;
    std::string getPreviousHash() const { return prevHash; }
    friend std::ostream& operator<<(std::ostream& os, const Block& block);
    bool validateTransactions(const UTXOManager& utxos) const;
    
private:
    int index;
    long timestamp;
    std::vector<Transaction> transactions;
    std::string prevHash;
    std::string hash;
    int nonce;
};