# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(mini_bitcoin)

set(CMAKE_CXX_STANDARD 17)

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
    main.cpp
    Block.cpp
    sha256.cpp
    Transaction.cpp
    UTXOManager.cpp
)

target_link_libraries(mini_bitcoin OpenSSL::Crypto OpenSSL::SSL)
