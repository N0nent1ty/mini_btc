// 初始 main.cpp（第一階段）
#include "Block.h"
#include "Transaction.h"
#include "UTXOManager.h"
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

    // 建立創世區塊（暫時不使用交易物件）
    Block genesis(0, {}, "0");
    genesis.mineBlock(3);

    std::cout << "Genesis Block Mined:\n" << genesis << std::endl;

    // 建立一筆模擬交易：Alice 轉帳 5 BTC 給 Bob
    Transaction coinbase({}, { TxOut{"Alice", 10.0} });
    utxoManager.addTransaction(coinbase);

    TxIn in{ coinbase.id, 0 };
    TxOut out1{ "Bob", 5.0 };
    TxOut out2{ "Alice", 5.0 }; // 找零回 Alice
    Transaction tx1({ in }, { out1, out2 });

    if (utxoManager.isUnspent(in)) {
        utxoManager.addTransaction(tx1);
        std::cout << "\n=== Transaction from Alice to Bob ===\n";
        std::cout << "TxID: " << tx1.id << "\n";
    } else {
        std::cout << "\n[!] Invalid transaction: Input already spent or nonexistent.\n";
    }

    std::cout << "\n=== UTXO Snapshot ===\n";
    std::cout << "Alice Balance: " << utxoManager.getBalance("Alice") << " BTC\n";
    std::cout << "Bob Balance:   " << utxoManager.getBalance("Bob") << " BTC\n";

    uint64_t previousDifficulty = 1000000000;
    uint64_t actualTimeTaken = 12 * 24 * 60 * 60;
    uint64_t newDifficulty = adjustDifficulty(previousDifficulty, actualTimeTaken);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n=== Difficulty Adjustment Simulation ===\n";
    std::cout << "Previous Difficulty: " << previousDifficulty << "\n";
    std::cout << "Actual Time Taken: " << actualTimeTaken / 3600.0 << " hours\n";
    std::cout << "Expected Time: " << EXPECTED_TIME / 3600.0 << " hours\n";
    std::cout << "New Difficulty: " << newDifficulty << "\n";

    return 0;
}
