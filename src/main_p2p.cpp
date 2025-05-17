#include "./p2p/Node.h"
#include "./p2p/NetworkSimulator.h"
#include "./crypto/Crypto.h"
#include <iostream>

int main() {
    NetworkSimulator network;

    // 建立三個節點
    Node alice("Alice_Node", &network);
    Node bob("Bob_Node", &network);
    Node miner("Miner_Node", &network);

    network.addNode(&alice);
    network.addNode(&bob);
    network.addNode(&miner);

    // 產生金鑰
    auto [alicePriv, alicePub] = generateKeyPair();
    auto [bobPriv, bobPub] = generateKeyPair();
    auto [minerPriv, minerPub] = generateKeyPair();

    // 創世區塊 (由 Miner 產生)
    Transaction genesisCoinbase({}, { TxOut{ alicePub, 10.0 } });
    Block genesisBlock(0, { genesisCoinbase }, "0");
    genesisBlock.mineBlock(3);

    // 模擬 Miner 直接產生 genesis block
    miner.receiveBlock(genesisBlock);

    // Alice 發交易給 Bob
    TxIn input{ genesisCoinbase.id, 0, "", "" };
    TxOut toBob{ bobPub, 5.0 };
    TxOut changeToAlice{ alicePub, 4.9 };

    Transaction tx({input}, {toBob, changeToAlice});
    std::string hash = tx.calculateHash();
    std::string sig = signMessage(alicePriv, hash);
    tx.inputs[0].signature = sig;
    tx.inputs[0].publicKey = alicePub;

    // Alice 廣播交易
    alice.receiveTransaction(tx);

    // Miner 從 mempool 選擇交易並打包
    auto blockTxs = miner.mempool.selectTransactions(1);
    Transaction coinbase({}, { TxOut{ minerPub, 6.25 + 0.1 } });
    std::vector<Transaction> fullTxs = { coinbase };
    fullTxs.insert(fullTxs.end(), blockTxs.begin(), blockTxs.end());

    Block newBlock(1, fullTxs, miner.blockchain.getLatestBlock().calculateHash());
    newBlock.mineBlock(3);

    miner.receiveBlock(newBlock);

    std::cout << "\n=== Simulation complete ===\n";
}