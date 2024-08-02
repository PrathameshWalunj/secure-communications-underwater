#include "FracturedMessage.h"
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <numeric>

FracturedMessage::FracturedMessage() 
    : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}

void FracturedMessage::fracture(const string& message, size_t numFragments) {
    fragments.clear();
    size_t fragmentSize = message.length() / numFragments;
    
    for (size_t i = 0; i < numFragments; ++i) {
        size_t start = i * fragmentSize;
        size_t end = (i == numFragments - 1) ? message.length() : start + fragmentSize;
        string fragment = message.substr(start, end - start);
        uint32_t checksum = calculateChecksum(fragment);
        fragments.emplace_back(i, fragment, checksum);
    }

    shuffle(fragments.begin(), fragments.end(), rng);
}

vector<tuple<int, string, uint32_t>>& FracturedMessage::getFragments() {
    return fragments;
}

string FracturedMessage::reassemble() const {
    if (!isComplete()) {
        throw runtime_error("Cannot reassemble incomplete message");
    }

    vector<tuple<int, string, uint32_t>> sortedFragments = fragments;
    sort(sortedFragments.begin(), sortedFragments.end());
    
    string result;
    for (const auto& fragment : sortedFragments) {
        if (!verifyFragment(get<0>(fragment), get<1>(fragment), get<2>(fragment))) {
            throw runtime_error("Fragment verification failed during reassembly");
        }
        result += get<1>(fragment);
    }
    return result;
}

void FracturedMessage::addFragment(int index, const string& fragment, uint32_t checksum) {
    fragments.emplace_back(index, fragment, checksum);
}

bool FracturedMessage::isComplete() const {
    if (fragments.empty()) {
        return false;
    }

    vector<bool> fragmentPresent(fragments.size(), false);
    for (const auto& fragment : fragments) {
        if (get<0>(fragment) >= static_cast<int>(fragments.size()) || get<0>(fragment) < 0) {
            return false;
        }
        fragmentPresent[get<0>(fragment)] = true;
    }

    return all_of(fragmentPresent.begin(), fragmentPresent.end(), [](bool present) { return present; });
}

uint32_t FracturedMessage::calculateChecksum(const string& data) {
    return accumulate(data.begin(), data.end(), 0u);
}

bool FracturedMessage::verifyFragment(int index, const string& fragment, uint32_t checksum) const {
    return calculateChecksum(fragment) == checksum;
}