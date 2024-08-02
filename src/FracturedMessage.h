#ifndef FRACTURED_MESSAGE_H
#define FRACTURED_MESSAGE_H

#include <vector>
#include <string>
#include <random>
#include <cstdint>

using namespace std;

class FracturedMessage {
private:
    vector<tuple<int, string, uint32_t>> fragments;
    mt19937 rng;

    static uint32_t calculateChecksum(const string& data);

public:
    FracturedMessage();
    void fracture(const string& message, size_t numFragments);
    vector<tuple<int, string, uint32_t>>& getFragments();
    string reassemble() const;
    void addFragment(int index, const string& fragment, uint32_t checksum);
    bool isComplete() const;
    bool verifyFragment(int index, const string& fragment, uint32_t checksum) const;
};

#endif 