## Cryptography Final Bank Project

### Requirements to Build/Run

* Boost
* Protobuf
* CMake

### System Tested On

This project was tested on Ubuntu 22.04 which we confirmed it worked.
We have not tested on other systems so cannot guarantee its
functionality on anything else.

### How to Install Requirements (on Ubuntu)

1. `sudo apt install cmake`
2. `sudo apt install libboost-all-dev`
3. `sudo apt install protobuf-compiler` (this should install relevant library needed)

### How to Build

Start from project root directory

1. Run: `cd proto`
2. Run: `protoc -I=. --cpp_out=. messages.proto`
3. Run: `cd ..`
4. Make a build directory: `mkdir build`
5. Run: `cmake -S . -B build`
6. Run: `cd build`
7. Run: `make`
8. You should now have an ATM and Bank executable in the build directory

### How to Run

1. Be in the project root directory (not in build otherwise it won't work)
2. Run `./build/Bank 1500` to start the Bank (must be started first)
3. Run `./build/ATM 1500` to start the Bank

Note: 1500 is the port number that will be used. This can be substituted
with any other port you would like. If you get any errors about address
already in use then use a different port.
