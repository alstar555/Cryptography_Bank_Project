cmake_minimum_required(VERSION 3.22)
project(Cryptography_Bank_Project)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)

find_package(Boost REQUIRED)

add_executable(ATM
        ATM.cpp crypto/SHA-1.cpp Utils.cpp)
target_link_libraries(ATM PRIVATE Boost::boost)

add_executable(Bank
        Bank.cpp crypto/SHA-1.cpp Client.cpp crypto/DESKey.cpp crypto/DH.cpp Utils.cpp)
target_link_libraries(Bank PRIVATE Boost::boost)
