#include "CommunicationChannel.h"
#include <thread>
#include <algorithm>

using namespace std;

CommunicationChannel::CommunicationChannel(double noise, double packetLoss, int minLat, int maxLat)
    : rng(chrono::steady_clock::now().time_since_epoch().count()),
      noiseProbability(noise),
      packetLossProbability(packetLoss),
      minLatency(minLat),
      maxLatency(maxLat) {}

string CommunicationChannel::introduceNoise(const string& message) {
    string noisyMessage = message;
    uniform_real_distribution<> distr(0.0, 1.0);
    
    for (char& c : noisyMessage) {
        if (distr(rng) < noiseProbability) {
            c = static_cast<char>(uniform_int_distribution<>(0, 255)(rng));
        }
    }
    
    return noisyMessage;
}

bool CommunicationChannel::isPacketLost() {
    uniform_real_distribution<> distr(0.0, 1.0);
    return distr(rng) < packetLossProbability;
}

int CommunicationChannel::getLatency() {
    uniform_int_distribution<> distr(minLatency, maxLatency);
    return distr(rng);
}

string CommunicationChannel::transmit(const string& message) {
    if (isPacketLost()) {
        return "";  // Packet lost, return empty string
    }
    
    string transmittedMessage = introduceNoise(message);
    
    // Simulate latency (reduced for testing purposes)
    this_thread::sleep_for(chrono::milliseconds(getLatency()));
    
    return transmittedMessage;
}
