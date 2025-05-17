// Node.h
#pragma once
#include "../blockchain/BlockChain.h"
#include "../utxomanager/UTXOManager.h"
#include "../mempool/Mempool.h"
#include "../transaction/Transaction.h"
#include "../blockchain/Block.h"
#include <string>
#include <vector>

class NetworkSimulator; 

class Node {
public:
    std::string nodeId;
    Blockchain blockchain;
    UTXOManager utxoManager;
    Mempool mempool;
    NetworkSimulator* network; 

    Node(const std::string& id, NetworkSimulator* net);

    void receiveTransaction(const Transaction& tx);
    void receiveBlock(const Block& block);

    void broadcastTransaction(const Transaction& tx);
    void broadcastBlock(const Block& block);
};