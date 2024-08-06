#ifndef COMMUNICATION_CHANNEL_H
#define COMMUNICATION_CHANNEL_H

#include <string>
#include <random>
#include <chrono>

using namespace std;

class CommunicationChannel {
private:
    mt19937 rng;
    double noiseProbability;
    double packetLossProbability;
    int minLatency;
    int maxLatency;

    string introduceNoise(const string& message);
    bool isPacketLost();
    int getLatency();

public:
    CommunicationChannel(double noise = 0.01, double packetLoss = 0.05, int minLat = 10, int maxLat = 100);
    string transmit(const string& message);
};

#endif 