// 初始 main.cpp（第一階段）
#include "Block.h"
#include "Transaction.h"
#include "UTXOManager.h"
#include "crypto.h"
#include <iostream>
#include <iomanip>

constexpr int BLOCK_INTERVAL_MINUTES = 10;
constexpr int DIFFICULTY_ADJUSTMENT_INTERVAL = 2016;
constexpr int EXPECTED_TIME = BLOCK_INTERVAL_MINUTES * DIFFICULTY_ADJUSTMENT_INTERVAL * 60; // 秒


uint64_t adjustDifficulty(uint64_t previousDifficulty, uint64_t actualTimeTakenSeconds) {
    const double maxFactor = 4.0;
    const double minFactor = 0.25;

    double ratio = static_cast<double>(actualTimeTakenSeconds) / EXPECTED_TIME;
    if (ratio > maxFactor) ratio = maxFactor;
    if (ratio < minFactor) ratio = minFactor;

    return static_cast<uint64_t>(previousDifficulty * ratio);
}


int main() {
    UTXOManager utxoManager;

    auto [alicePrivKey, alicePubKey] = generateKeyPair();
    std::cout << "Alice Private Key: " << alicePrivKey << "\n";
    std::cout << "Alice Public Key:  " << alicePubKey << "\n";
    
    // 2. 產生 Bob 的金鑰對（用來收錢，但這裡不需要簽名）
    auto [bobPrivKey, bobPubKey] = generateKeyPair();
    std::cout << "Bob Public Key:  " << bobPubKey << "\n";
    
    // 3. 假設 Alice 有一筆先前coinbase得到的UTXO
    Transaction coinbase({}, { TxOut{ alicePubKey, 10.0 } });
    utxoManager.addTransaction(coinbase);
    
    // 4. Alice 發出一筆新交易：轉5 BTC給Bob，剩餘自己找零
    TxIn input{ coinbase.id, 0, "", "" };
    TxOut outputToBob{ bobPubKey, 5.0 };
    TxOut changeBackToAlice{ alicePubKey, 5.0 };
    Transaction tx({ input }, { outputToBob, changeBackToAlice });
    
    // 5. Alice 對交易進行簽名
    std::string messageHash = tx.calculateHash();
    std::string signature = signMessage(alicePrivKey, messageHash);
    
    // 6. 填入TxIn的簽名與公鑰
    tx.inputs[0].signature = signature;
    tx.inputs[0].publicKey = alicePubKey;
    
    // 7. 讓 UTXOManager 驗證交易
    if (utxoManager.validateTransaction(tx)) {
        std::cout << "\nTransaction validated successfully!\n";
        utxoManager.addTransaction(tx);
    
        std::cout << "\n=== Final Balances ===\n";
        std::cout << "Alice: " << utxoManager.getBalance(alicePubKey) << " BTC\n";
        std::cout << "Bob:   " << utxoManager.getBalance(bobPubKey) << " BTC\n";
    } else {
        std::cout << "\n[!] Transaction validation failed!\n";
    }
}
