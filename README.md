## Cryptography Final Bank Project

### Requirements to Build/Run

* Boost
* Protobuf
* CMake

### How to Install Requirements (on Ubuntu)

1. `sudo apt install cmake`
2. `sudo apt install libboost-all-dev`
3. `sudo apt install protobuf-compiler` (this should install relevant library needed)

### How to Build

1. Make a build directory: `mkdir build`
2. Run: `cmake -S . -B build`
3. Run: `cd build`
4. Run: `make`
5. You should now have an ATM and Bank executable in the build directory

### How to Run

1. Be in the project root directory (not in build)
2. Run `./build/Bank 1500` to start the Bank (must be started first)
3. Run `./build/ATM 1500` to start the Bank

Note: 1500 is the port number that will be used. This can be substituted
with any other port you would like. If you get any errors about address
already in use then use a different port.
