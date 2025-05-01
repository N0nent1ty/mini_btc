
# Set C++ standard and architecture
set(CMAKE_CXX_STANDARD 17)
# Set the architecture to x64
set(CMAKE_GENERATOR_PLATFORM x64)



add_executable(test_block_validation
    test_block_validation.cpp
)


target_sources(test_block_validation PRIVATE
    ${CMAKE_SOURCE_DIR}/src/blockchain/Block.cpp
    ${CMAKE_SOURCE_DIR}/src/blockchain/Blockchain.cpp
    ${CMAKE_SOURCE_DIR}/src/transaction/Transaction.cpp
    ${CMAKE_SOURCE_DIR}/src/utxomanager/UTXOManager.cpp
    ${CMAKE_SOURCE_DIR}/src/mempool/Mempool.cpp
    ${CMAKE_SOURCE_DIR}/src/crypto/Crypto.cpp
    ${CMAKE_SOURCE_DIR}/src/sha256/sha256.cpp
)


target_include_directories(test_block_validation PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/blockchain
    ${CMAKE_SOURCE_DIR}/src/transaction
    ${CMAKE_SOURCE_DIR}/src/utxomanager
    ${CMAKE_SOURCE_DIR}/src/mempool
    ${CMAKE_SOURCE_DIR}/src/crypto
    ${CMAKE_SOURCE_DIR}/src/sha256
)

find_package(OpenSSL REQUIRED)
target_link_libraries(test_block_validation PRIVATE OpenSSL::Crypto OpenSSL::SSL ws2_32 crypt32)