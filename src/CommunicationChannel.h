#ifndef COMMUNICATION_CHANNEL_H
#define COMMUNICATION_CHANNEL_H

#include "PythonIntegration.h"
#include <string>
#include <random>
#include <chrono>

class CommunicationChannel {
private:
    std::mt19937 rng;
    double noiseProbability;
    double packetLossProbability;
    int minLatency;
    int maxLatency;
    PythonIntegration pythonIntegration;
    double currentFrequency;
    int currentChannel;

    std::string introduceNoise(const std::string& message);
    bool isPacketLost();
    int getLatency();

public:
    CommunicationChannel(double noise = 0.01, double packetLoss = 0.05, int minLat = 10, int maxLat = 100);
    std::string transmit(const std::string& message);
    double getCurrentFrequency() const { return currentFrequency; }
};

#endif 