cmake_minimum_required(VERSION 3.22)
project(Cryptography_Bank_Project)

set(CMAKE_CXX_STANDARD 14)

add_executable(ATM
        ATM.cpp)

add_executable(Bank
        Bank.cpp)
