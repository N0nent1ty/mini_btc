# Mini Bitcoin

This repository is created for learning about blockchain technology by reproducing the core concepts of Bitcoin (BTC). The project implements a simplified version of Bitcoin to understand its fundamental components, such as blocks, transactions, mining, and cryptographic hashing.

## Features

- **Blockchain Basics**: Learn how blocks are linked together in a chain.
- **Transactions**: Simulate Bitcoin-like transactions.
- **Proof-of-Work (PoW)**: Implement mining with adjustable difficulty.
- **SHA-256 Hashing**: Use cryptographic hashing for block integrity.
- **UTXO Management**: Manage unspent transaction outputs (UTXOs).

## Project Structure

- `main.cpp`: Entry point of the application.
- `Block.cpp` and `Block.h`: Implementation of the `Block` class, including mining and hashing.
- `Transaction.cpp` and `Transaction.h`: Implementation of transactions.
- `sha256.cpp` and `sha256.h`: SHA-256 hashing algorithm.
- `crypto.cpp` and `crypto.h`: Additional cryptographic utilities.
- `UTXOManager.cpp` and `UTXOManager.h`: Manage UTXOs for transaction validation.
- `CMakeLists.txt`: Build configuration for the project.
- `vcpkg.json`: Dependency management configuration for vcpkg.

## Prerequisites

- **C++17**: Ensure your compiler supports C++17.
- **CMake**: Build system for the project.
- **OpenSSL**: Required for cryptographic operations (installed via vcpkg).

## Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/mini_bitcoin.git
   cd mini_bitcoin
   ```
2. Install dependencies using vcpkg:
    ```bash
    vcpkg install openssl:x64-windows
    ```
3. Configure and build the project:
    ```bash
    mkdir build
    cd build
    cmake -DCMAKE_TOOLCHAIN_FILE="C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake" ..
    cmake --build .
    ```
4. Run the executable:
    ```bash
    .\Debug\mini_bitcoin.exe
    ```

## Contributing

Contributions are welcome! If you have suggestions for improvements or new features, feel free to open an issue or submit a pull request.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.