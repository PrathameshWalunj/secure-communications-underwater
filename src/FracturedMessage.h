#ifndef FRACTURED_MESSAGE_H
#define FRACTURED_MESSAGE_H

#include <vector>
#include <string>
#include <random>
#include <cstdint>

class FracturedMessage {
private:
    std::vector<std::tuple<int, std::string, uint32_t>> fragments;
    std::mt19937 rng;

    static uint32_t calculateCRC32(const std::string& data);

public:
    FracturedMessage();
    void fracture(const std::string& message, size_t numFragments);
    std::vector<std::tuple<int, std::string, uint32_t>>& getFragments();
    const std::vector<std::tuple<int, std::string, uint32_t>>& getFragments() const; // Add this line
    std::string reassemble() const;
    void addFragment(int index, const std::string& fragment, uint32_t crc);
    bool isComplete() const;
    bool verifyFragment(int index, const std::string& fragment, uint32_t crc) const;
};

#endif