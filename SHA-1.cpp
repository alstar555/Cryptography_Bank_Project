#include <string>
#include <iostream>
#include <cstdint>
#include <bit>
#include <vector>
#include <iomanip>


class SHA1 {
private:
    const uint32_t BLOCK_LEN = 64; //bytes

    std::vector<uint8_t> add_padding(const std::string& message) {
        std::vector<uint8_t> bytes(message.begin(), message.end());
        
        // Append the bit '1' to the message by adding 0x80
        bytes.emplace_back(static_cast<uint8_t>(0x80));

        // Append'0' until the total length of msg is a multiple of 64-8 bytes 
        while((bytes.size()+8) % BLOCK_LEN != 0){
            bytes.emplace_back(static_cast<uint8_t>(0x00));
        }

        uint64_t bit_length = 8 * message.length();
        for ( int32_t i = 7; i >= 0; --i ) {
            bytes.emplace_back(static_cast<int8_t>(bit_length >> ( 8 * i )));
        }

        return bytes;
    }

    uint32_t left_rotate(uint32_t n, int shift, int b){
        return (n << n) | (n >> b-n);
    }

    

  
public:
    // Generate a 160 bit message digest
    std::string operator()(const std::string& message) {
        std::string msg_hashed = "";

        // Init hash H values
        uint32_t H0 = 0x67452301;
        uint32_t H1 = 0xEFCDAB89;
        uint32_t H2 = 0x98BADCFE;
        uint32_t H3 = 0x10325476;
        uint32_t H4 = 0xC3D2E1F0;

        // Msg padded to divide 64 bytes
        const std::vector<uint8_t> padded_msg = add_padding(message);

        // Divide mssg into 64 byte chunks
        for(uint64_t i = 0; i < padded_msg.size()/BLOCK_LEN; i++) {
            std::vector<uint32_t> w(80, 0);
            for(uint32_t j = 0; j < BLOCK_LEN; j++) {
                 // Divide chunks into 16 4-byte words
                w[j/4] |= (padded_msg[i * BLOCK_LEN + j])  << ( ( 3 - j % 4 ) * 8 );


            }
            // Extend the sixteen words into eighty words:
            for(int i = 16; i!= 80; i++){
                w[i] = left_rotate(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1, 32);
            }


            uint32_t a = H0;
            uint32_t b = H1;
            uint32_t c = H2;
            uint32_t d = H3;
            uint32_t e = H4;
            uint32_t f=0;
            uint32_t k=0;
            uint32_t temp;

            // Bit manipulations 
            for(int j = 0; j!= 80; j++){


                if (0 <= j && j <= 19){
                    f = (b & c) | (~b & d);
                    k = 0x5A827999;
                }
                else if (20 <= j && j <= 39){
                    f = b ^ c ^ d;
                    k = 0x6ED9EBA1;
                }
                else if (40 <= j && j <= 59){
                    f = (b & c) | (b & d) | (c & d);
                    k = 0x8F1BBCDC;
                }
                else if (60 <= j && j <= 79){
                    f = b ^ c ^ d;
                    k = 0xCA62C1D6;
                }
                temp = left_rotate(a, 5, 32) + f + e + k + w[j];
       
                e = d;
                d = c;
                c = left_rotate(b, 30, 32);
                b = a;
                a = temp;

            }
            // Append results to hash
            H0 += a;
            H1 += b; 
            H2 += c;
            H3 += d;
            H4 += e;
        }

         // Concatenate the hashed blocks into 160-bit number
        std::stringstream HH_stream;
        HH_stream << std::hex << std::setw(8) << std::setfill('0') << H0;
        HH_stream << std::hex << std::setw(8) << std::setfill('0') << H1;
        HH_stream << std::hex << std::setw(8) << std::setfill('0') << H2;
        HH_stream << std::hex << std::setw(8) << std::setfill('0') << H3;
        HH_stream << std::hex << std::setw(8) << std::setfill('0') << H4;

        return HH_stream.str();
    }
    
    
};



int main() {
  
    SHA1 sha;

    std::cout << sha("Testing testing 123 ...");
    std::cout << std::endl;

  
    return 0;
}