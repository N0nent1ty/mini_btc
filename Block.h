// Block.h
#pragma once
#include "Transaction.h"
#include <string>
#include <vector>
#include <ctime>
#include <iostream>

class Block {
public:
    Block(int index, const std::vector<Transaction>& transactions, const std::string& prevHash);

    void mineBlock(int difficulty);
    std::string calculateHash() const;

    friend std::ostream& operator<<(std::ostream& os, const Block& block);

private:
    int index;
    long timestamp;
    std::vector<Transaction> transactions;
    std::string prevHash;
    std::string hash;
    int nonce;
};