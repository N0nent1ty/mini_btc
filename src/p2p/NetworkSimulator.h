// NetworkSimulator.h
#pragma once
#include "../transaction/Transaction.h"
#include "../blockchain/Block.h"
#include <vector>

class Node; // 前向宣告

class NetworkSimulator {
public:
    std::vector<Node*> nodes;

    void addNode(Node* node);

    void broadcastTransaction(Node* from, const Transaction& tx);
    void broadcastBlock(Node* from, const Block& block);
};