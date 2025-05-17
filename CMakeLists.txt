# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(mini_bitcoin)

add_subdirectory(src/Unittest)

# Set C++ standard and architecture
set(CMAKE_CXX_STANDARD 17)
# Set the architecture to x64
set(CMAKE_GENERATOR_PLATFORM x64)


# Windows 專用建構提示
if(WIN32)
    message(STATUS "Building on Windows")
endif()

# vcpkg support for Windows
set(OPENSSL_ROOT_DIR "C:/Users/sheng/vcpkg/installed/x64-windows-static")
set (VCPKG_ROOT "C:/Users/sheng/vcpkg" CACHE PATH "Path to vcpkg root directory")
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

# Print debug message for CMAKE_TOOLCHAIN_FILE
message(STATUS "CMAKE_TOOLCHAIN_FILE is set to: ${CMAKE_TOOLCHAIN_FILE}")

find_package(OpenSSL REQUIRED)

add_executable(mini_bitcoin
    src/main.cpp
    src/blockchain/Block.cpp
    src/blockchain/BlockChain.cpp
    src/utxomanager/UTXOManager.cpp
    src/sha256/SHA256.cpp
    src/crypto/Crypto.cpp
    src/transaction/Transaction.cpp
    src/mempool/Mempool.cpp
)

target_link_libraries(mini_bitcoin OpenSSL::Crypto OpenSSL::SSL ws2_32 crypt32)



# Build main_p2p 
add_executable(main_p2p
    src/main_p2p.cpp
    src/blockchain/Block.cpp
    src/blockchain/BlockChain.cpp
    src/utxomanager/UTXOManager.cpp
    src/sha256/SHA256.cpp
    src/crypto/Crypto.cpp
    src/transaction/Transaction.cpp
    src/mempool/Mempool.cpp
    src/p2p/Node.cpp
    src/p2p/NetworkSimulator.cpp
)

target_link_libraries(main_p2p OpenSSL::Crypto OpenSSL::SSL ws2_32 crypt32)