#include "./blockchain/Block.h"
#include "./blockchain/BlockChain.h"
#include "./transaction/Transaction.h"
#include "./utxomanager/UTXOManager.h"
#include "./crypto/Crypto.h"
#include "./mempool/Mempool.h"
#include <iostream>
#include <iomanip>

constexpr double BLOCK_REWARD = 6.25; // 比特幣目前獎勵（可依時代調整）
constexpr int BLOCK_INTERVAL_MINUTES = 10;
constexpr int DIFFICULTY_ADJUSTMENT_INTERVAL = 2016;
constexpr int EXPECTED_TIME = BLOCK_INTERVAL_MINUTES * DIFFICULTY_ADJUSTMENT_INTERVAL * 60; // 秒

// 根據交易計算手續費：inputs 金額總和 - outputs 金額總和
double calculateTransactionFee(const Transaction& tx, const UTXOManager& utxos) {
    double inputSum = 0.0;
    for (const auto& in : tx.inputs) {
        std::optional<TxOut> utxo = utxos.getUTXO(in.prevTxId, in.outputIndex);
        if (utxo.has_value()) {
            inputSum += utxo->amount;
        }
    }

    double outputSum = 0.0;
    for (const auto& out : tx.outputs) {
        outputSum += out.amount;
    }

    return inputSum - outputSum;
}

int main() {
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

    // 3. 產生礦工的交易金鑰對（用來挖礦） 
    auto [minerPrivKey, minerPubKey] = generateKeyPair();

    // 4. 假設 Alice 有一筆先前coinbase得到的UTXO (創世區塊)
    Transaction genesisCoinbase({}, {TxOut{alicePubKey, 10.0}});
    Block genesisBlock(0, {genesisCoinbase}, "0");
    genesisBlock.mineBlock(3);         // 模擬挖礦，難度為3
    blockchain.addBlock(genesisBlock, utxoManager); // 將創世區塊加入區塊鏈
    utxoManager.addTransaction(genesisCoinbase);

    // 5. Alice 發出一筆新交易：轉5 BTC給Bob，剩餘自己找零
    TxIn input{genesisCoinbase.id, 0, "", ""};
    TxOut outputToBob{bobPubKey, 5.0};
    TxOut changeBackToAlice{ alicePubKey, 4.7 }; // 手續費 0.3 BTC
    Transaction tx({input}, {outputToBob, changeBackToAlice});

    // 6. Alice 對交易進行簽名
    std::string messageHash = tx.calculateHash();
    std::string signature = signMessage(alicePrivKey, messageHash);
    tx.inputs[0].signature = signature;
    tx.inputs[0].publicKey = alicePubKey;

    // 7. 讓 UTXOManager 驗證交易
    if (utxoManager.validateTransaction(tx)) {
        mempool.addTransaction(tx); // 將有效交易放入 mempool
        std::cout << "\nTransaction validated and added to mempool!\n";
    } else {
        std::cout << "\n[!] Transaction validation failed!\n";
    }

    // 8. 模擬礦工從 mempool 中取出交易打包區塊
    std::vector<Transaction> blockTxs = mempool.selectTransactions(1); // 目前選一筆

    // 9. 礦工從 mempool 中選擇交易並計算總手續費
    double totalFees = 0.0;
    for (const auto& t : blockTxs) {
        totalFees += calculateTransactionFee(t, utxoManager);
    }

    // 10. 建立 coinbase 交易（獎勵給礦工）
    Transaction coinbaseTx({}, { TxOut{ minerPubKey, BLOCK_REWARD + totalFees } });

    // 11. 打包成區塊（coinbase + 一般交易）
    std::vector<Transaction> fullBlockTxs = { coinbaseTx };
    fullBlockTxs.insert(fullBlockTxs.end(), blockTxs.begin(), blockTxs.end());
    Block newBlock(1, fullBlockTxs, blockchain.getLatestBlock().calculateHash());
    newBlock.mineBlock(3);

    // 12. 嘗試將區塊加入區塊鏈
    if (blockchain.addBlock(newBlock, utxoManager)) {
        for (const auto& tx : fullBlockTxs) {
            utxoManager.addTransaction(tx);
            mempool.removeTransaction(tx.id);
        }
    } else {
        std::cerr << "[!] Block rejected and not added to blockchain.\n";
    }

    // 13. 印出完整區塊鏈與餘額
    std::cout << "\n=== Blockchain ===\n";
    blockchain.print();

    std::cout << "\n=== Final Balances ===\n";
    std::cout << "Alice: " << utxoManager.getBalance(alicePubKey) << " BTC\n";
    std::cout << "Bob:   " << utxoManager.getBalance(bobPubKey) << " BTC\n";
    std::cout << "Miner: " << utxoManager.getBalance(minerPubKey) << " BTC\n";

    return 0;
}
