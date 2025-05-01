// Blockchain.h
#pragma once
#include "Block.h"
#include "../utxomanager/UTXOManager.h"
#include <vector>

class Blockchain {
public:
    bool addBlock(const Block& block, const UTXOManager& utxos);
    const Block& getLatestBlock() const;
    bool isValidChain() const;
    void print() const;

private:
    std::vector<Block> chain;
};