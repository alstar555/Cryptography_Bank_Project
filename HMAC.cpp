#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "Utils.hpp"
#include "SHA-1.h"
#include "string.h"
#include <boost/multiprecision/cpp_int.hpp>

class HMAC {
public:
	int block_size = 64;

    std::string operator()(const std::string& key, const std::string& message){
	    std::string resized_key = resize_key(key);
	    std::string outer_key_padded = xor_string(resized_key, std::string(block_size, '\x5c'));
	    std::string inner_key_padded = xor_string(resized_key, std::string(block_size, '\x36'));
    	return hash(outer_key_padded + hash(inner_key_padded + message));
    }

private:	
    SHA1 sha1;
	
	std::string hash(const std::string& msg) {
        return sha1(msg);
    }

    std::string xor_string(const std::string& a, const std::string& b) {
        std::string c;
        for (size_t i = 0; i < a.size(); ++i) {
            c.push_back(a[i] ^ b[i]);
        }
        return c;
    }

    std::string resize_key(const std::string& key) {
    	if (key.length() > block_size){
        	return hash(key);
    	}
        if (key.length() > block_size){
        	return key + std::string(block_size - key.length(), '\x00');
    	}
    	return key;
    }
};

int main() {
    HMAC hmac;
    std::cout << hmac("key", "Hello Bro") << std::endl;
    return 0;
}
