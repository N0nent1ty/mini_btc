#include "./blockchain/Block.h"
#include "./blockchain/BlockChain.h"
#include "./transaction/Transaction.h"
#include "./utxomanager/UTXOManager.h"
#include "./crypto/Crypto.h"
#include "./mempool/Mempool.h"
#include <iostream>
#include <iomanip>

constexpr int BLOCK_INTERVAL_MINUTES = 10;
constexpr int DIFFICULTY_ADJUSTMENT_INTERVAL = 2016;
constexpr int EXPECTED_TIME = BLOCK_INTERVAL_MINUTES * DIFFICULTY_ADJUSTMENT_INTERVAL * 60; // 秒

uint64_t adjustDifficulty(uint64_t previousDifficulty, uint64_t actualTimeTakenSeconds)
{
    const double maxFactor = 4.0;
    const double minFactor = 0.25;

    double ratio = static_cast<double>(actualTimeTakenSeconds) / EXPECTED_TIME;
    if (ratio > maxFactor)
        ratio = maxFactor;
    if (ratio < minFactor)
        ratio = minFactor;

    return static_cast<uint64_t>(previousDifficulty * ratio);
}

int main()
{
    UTXOManager utxoManager;
    Mempool mempool;       // 新增 mempool 實例
    Blockchain blockchain; // 新增 blockchain 模組

    // 1. 產生 Alice 的金鑰對（用來簽名）
    auto [alicePrivKey, alicePubKey] = generateKeyPair();
    std::cout << "Alice Private Key: " << alicePrivKey << "\n";
    std::cout << "Alice Public Key:  " << alicePubKey << "\n";

    // 2. 產生 Bob 的金鑰對（用來收錢，但這裡不需要簽名）
    auto [bobPrivKey, bobPubKey] = generateKeyPair();
    std::cout << "Bob Public Key:  " << bobPubKey << "\n";

    // 3. 假設 Alice 有一筆先前coinbase得到的UTXO (創世區塊)
    Transaction coinbase({}, {TxOut{alicePubKey, 10.0}});
    Block genesisBlock(0, {coinbase}, "0");
    genesisBlock.mineBlock(3);         // 模擬挖礦，難度為3
    blockchain.addBlock(genesisBlock); // 將創世區塊加入區塊鏈
    utxoManager.addTransaction(coinbase);

    // 4. Alice 發出一筆新交易：轉5 BTC給Bob，剩餘自己找零
    TxIn input{coinbase.id, 0, "", ""};
    TxOut outputToBob{bobPubKey, 5.0};
    TxOut changeBackToAlice{alicePubKey, 5.0};
    Transaction tx({input}, {outputToBob, changeBackToAlice});

    // 5. Alice 對交易進行簽名
    std::string messageHash = tx.calculateHash();
    std::string signature = signMessage(alicePrivKey, messageHash);

    // 6. 填入TxIn的簽名與公鑰
    tx.inputs[0].signature = signature;
    tx.inputs[0].publicKey = alicePubKey;

    // 7. 讓 UTXOManager 驗證交易
    if (utxoManager.validateTransaction(tx))
    {
        mempool.addTransaction(tx); // 將有效交易放入 mempool
        std::cout << "\nTransaction validated and added to mempool!\n";
    }
    else
    {
        std::cout << "\n[!] Transaction validation failed!\n";
    }

    // 8.模擬礦工從 mempool 中取出交易打包區塊
    std::vector<Transaction> blockTxs = mempool.selectTransactions(1); // 目前選一筆
                                                                       // Index, transactions, previousHash
    Block newBlock(1, blockTxs, blockchain.getLatestBlock().calculateHash());
    newBlock.mineBlock(3);
    std::cout << "\nMined New Block:\n"
              << newBlock << std::endl;

    blockchain.addBlock(newBlock); // 將新區塊加入區塊鏈

    // 9.將被打包的交易從 mempool 移除並更新 UTXO 狀態
    for (const auto &tx : blockTxs)
    {
        utxoManager.addTransaction(tx);
        mempool.removeTransaction(tx.id);
    }
    std::cout << "\n=== Blockchain ===\n";
    blockchain.print();

    std::cout << "\n=== Final Balances ===\n";
    std::cout << "Alice: " << utxoManager.getBalance(alicePubKey) << " BTC\n";
    std::cout << "Bob:   " << utxoManager.getBalance(bobPubKey) << " BTC\n";
}
