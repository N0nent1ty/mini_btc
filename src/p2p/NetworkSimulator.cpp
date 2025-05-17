// NetworkSimulator.cpp
#include "NetworkSimulator.h"
#include "Node.h"

void NetworkSimulator::addNode(Node* node) {
    nodes.push_back(node);
}

void NetworkSimulator::broadcastTransaction(Node* from, const Transaction& tx) {
    for (auto node : nodes) {
        if (node != from) {
            node->receiveTransaction(tx);
        }
    }
}

void NetworkSimulator::broadcastBlock(Node* from, const Block& block) {
    for (auto node : nodes) {
        if (node != from) {
            node->receiveBlock(block);
        }
    }
}
