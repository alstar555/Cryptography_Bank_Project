cmake_minimum_required(VERSION 3.22)
project(Cryptography_Bank_Project)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED)

add_executable(ATM
        ATM.cpp)
target_link_libraries(ATM PRIVATE Boost::boost)

add_executable(Bank
        Bank.cpp)
target_link_libraries(Bank PRIVATE Boost::boost)
