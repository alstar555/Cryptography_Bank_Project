cmake_minimum_required(VERSION 3.22)
project(Cryptography_Bank_Project)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)

find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)

add_executable(ATM
        ATM.cpp crypto/SHA-1.cpp Utils.cpp crypto/RSA.cpp proto/messages.pb.cc crypto/DH.cpp crypto/DESKey.cpp crypto/DES.cpp crypto/HMAC.cpp)
target_link_libraries(ATM PRIVATE Boost::boost)
target_link_libraries(ATM PRIVATE ${Protobuf_LIBRARIES})

add_executable(Bank
        Bank.cpp crypto/SHA-1.cpp Client.cpp crypto/DESKey.cpp crypto/RSA.cpp crypto/DH.cpp Utils.cpp proto/messages.pb.cc crypto/DES.cpp crypto/HMAC.cpp)
target_link_libraries(Bank PRIVATE Boost::boost)
target_link_libraries(Bank PRIVATE ${Protobuf_LIBRARIES})

