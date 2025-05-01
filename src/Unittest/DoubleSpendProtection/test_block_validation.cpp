#include "../../blockchain/Block.h"
#include "../../blockchain/BlockChain.h"
#include "../../utxomanager/UTXOManager.h"
#include "../../mempool/Mempool.h"
#include "../../transaction/Transaction.h"
#include "../../crypto/Crypto.h"
#include <cassert>
#include <iostream>

int main() {
    UTXOManager utxoManager;
    Blockchain chain;

    auto [alicePriv, alicePub] = generateKeyPair();
    auto [bobPriv, bobPub] = generateKeyPair();

    // 建立創世區塊
    Transaction coinbase({}, { TxOut{alicePub, 10.0} });
    Block genesis(0, { coinbase }, "0");
    genesis.mineBlock(3);
    assert(chain.addBlock(genesis, utxoManager));
    utxoManager.addTransaction(coinbase);

    // 建立第一筆交易 Alice → Bob
    TxIn input1{ coinbase.id, 0, "", "" };
    TxOut out1{ bobPub, 5.0 };
    TxOut change1{ alicePub, 5.0 };
    Transaction tx1({ input1 }, { out1, change1 });
    tx1.inputs[0].signature = signMessage(alicePriv, tx1.calculateHash());
    tx1.inputs[0].publicKey = alicePub;

    // 建立第二筆交易 Alice 雙花同一個 input
    TxIn input2{ coinbase.id, 0, "", "" };
    TxOut out2{ bobPub, 6.0 };
    Transaction tx2({ input2 }, { out2 });
    tx2.inputs[0].signature = signMessage(alicePriv, tx2.calculateHash());
    tx2.inputs[0].publicKey = alicePub;

    // 打包同一區塊
    Block doubleSpendBlock(1, { tx1, tx2 }, genesis.calculateHash());
    doubleSpendBlock.mineBlock(3);
    std::cout << "Double-spend block:\n" << doubleSpendBlock << "\n";

    // 應該被拒絕
    bool result = chain.addBlock(doubleSpendBlock, utxoManager);
    assert(result == false);
    std::cout << " Double-spend block was correctly rejected.\n";
}