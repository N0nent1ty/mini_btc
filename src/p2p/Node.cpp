// Node.cpp
#include "Node.h"
#include "NetworkSimulator.h"
#include <iostream>

Node::Node(const std::string& id, NetworkSimulator* net) : nodeId(id), network(net) {}

void Node::receiveTransaction(const Transaction& tx) {
    if (utxoManager.validateTransaction(tx)) {
        mempool.addTransaction(tx);
        std::cout << "[" << nodeId << "] Accepted transaction: " << tx.id << "\n";
        broadcastTransaction(tx);
    }
}

void Node::receiveBlock(const Block& block) {
    if (blockchain.addBlock(block, utxoManager)) {
        std::cout << "[" << nodeId << "] Accepted block: " << block.calculateHash() << "\n";
        broadcastBlock(block);
    } else {
        std::cout << "[" << nodeId << "] Rejected block\n";
    }
}

void Node::broadcastTransaction(const Transaction& tx) {
    if (network) {
        network->broadcastTransaction(this, tx);
    }
}

void Node::broadcastBlock(const Block& block) {
    if (network) {
        network->broadcastBlock(this, block);
    }
}
