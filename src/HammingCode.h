#ifndef HAMMING_CODE_H
#define HAMMING_CODE_H

#include <vector>
#include <cstdint>

using namespace std;

class HammingCode {
public:
    static vector<uint8_t> encode(const vector<uint8_t>& data);
    static vector<uint8_t> decode(const vector<uint8_t>& encoded);

private:
    static uint8_t calculateParity(uint8_t data);
    static uint8_t correctError(uint8_t data, uint8_t syndrome);
};

#endif 