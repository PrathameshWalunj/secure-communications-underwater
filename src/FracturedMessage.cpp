#include "FracturedMessage.h"
#include <algorithm>
#include <chrono>
#include <stdexcept>  
#include <array>
#include<iostream>

FracturedMessage::FracturedMessage() 
    : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

void FracturedMessage::fracture(const std::string& message, size_t numFragments) {
    fragments.clear();
    size_t fragmentSize = message.length() / numFragments;
    
    for (size_t i = 0; i < numFragments; ++i) {
        size_t start = i * fragmentSize;
        size_t end = (i == numFragments - 1) ? message.length() : start + fragmentSize;
        std::string fragment = message.substr(start, end - start);
        uint32_t crc = calculateCRC32(fragment);
        fragments.emplace_back(i, fragment, crc);
    }

    std::shuffle(fragments.begin(), fragments.end(), rng);
}

std::vector<std::tuple<int, std::string, uint32_t>>& FracturedMessage::getFragments() {
    return fragments;
}

const std::vector<std::tuple<int, std::string, uint32_t>>& FracturedMessage::getFragments() const {
    return fragments;

}

std::string FracturedMessage::reassemble() const {
    if (!isComplete()) {
        throw std::runtime_error("Cannot reassemble incomplete message");
    }

    std::vector<std::tuple<int, std::string, uint32_t>> sortedFragments = fragments;
    std::sort(sortedFragments.begin(), sortedFragments.end());
    
    std::string result;
    for (const auto& fragment : sortedFragments) {
        int index = std::get<0>(fragment);
        const std::string& fragmentData = std::get<1>(fragment);
        uint32_t storedCRC = std::get<2>(fragment);
        uint32_t calculatedCRC = calculateCRC32(fragmentData);
        
        std::cout << "Fragment " << index << " - Stored CRC: " << storedCRC 
                  << ", Calculated CRC: " << calculatedCRC << std::endl;
        
        if (storedCRC != calculatedCRC) {
            throw std::runtime_error("Fragment verification failed during reassembly for fragment " + std::to_string(index));
        }
        result += fragmentData;
    }
    return result;
}


void FracturedMessage::addFragment(int index, const std::string& fragment, uint32_t crc) {
    uint32_t calculatedCRC = calculateCRC32(fragment);
    std::cout << "Adding fragment " << index << " - Provided CRC: " << crc 
              << ", Calculated CRC: " << calculatedCRC << std::endl;
    
    if (crc != calculatedCRC) {
        throw std::runtime_error("CRC mismatch when adding fragment " + std::to_string(index));
    }
    
    fragments.emplace_back(index, fragment, crc);
}


bool FracturedMessage::isComplete() const {
    if (fragments.empty()) {
        return false;
    }

    std::vector<bool> fragmentPresent(fragments.size(), false);
    for (const auto& fragment : fragments) {
        if (std::get<0>(fragment) >= static_cast<int>(fragments.size()) || std::get<0>(fragment) < 0) {
            return false;
        }
        fragmentPresent[std::get<0>(fragment)] = true;
    }

    return std::all_of(fragmentPresent.begin(), fragmentPresent.end(), [](bool present) { return present; });
}

uint32_t FracturedMessage::calculateCRC32(const std::string& data) {
    static const std::array<uint32_t, 256> crc_table = {
        // CRC32 lookup table (generated with polynomial 0xEDB88320)
      0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
        0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
        0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
        0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
        0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
        0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
        0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
        0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
        0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
        0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
        0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
        0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
        0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
        0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
        0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
        0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
        0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
        0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
        0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
        0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7e9d5, 0x60b08ed1,
        0xd6d6a3e8, 0xa1d357a3, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
        0xd80d2bda, 0xaf0a1b4c, 0x360d1f8f, 0x41047a60, 0xdf60efc3, 0xa867cf7d, 0x316e8fff, 0x4669c4f3,
        0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c6e04, 0xbb0b4706, 0x220216b9, 0x5505262f,
        0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7e8b7, 0xb5d0cf5d, 0x2cd99e1e, 0x5bde0d2a,
        0x9b64c2f4, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
        0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9c, 0xe5d5d9d7, 0x7cdcefb5, 0x0bdbdf21,
        0x86d3d2d7, 0xf1d4e242, 0x68ddb3f4, 0x1fda8360, 0x81be16cd, 0xf6b9265b, 0x6fd5a1b0, 0x18b74777,
        0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae4d, 0x616bffd1, 0x166ccf41,
        0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
        0xaed16a4a, 0xd9d65adc, 0x40df0b01, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5c6d5,
        0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de572f, 0x23d967bf,
        0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
    };  

    uint32_t crc = 0xFFFFFFFF;
    for (char c : data) {
        crc = (crc >> 8) ^ crc_table[(crc & 0xFF) ^ static_cast<uint8_t>(c)];
    }
    return ~crc;
}

bool FracturedMessage::verifyFragment(int index, const std::string& fragment, uint32_t crc) const {
    return calculateCRC32(fragment) == crc;
}