#include "CommunicationChannel.h"
#include <thread>
#include <algorithm>
#include<iostream>
#include<sstream>
#include<iomanip>

using namespace std;

CommunicationChannel::CommunicationChannel(double noise, double packetLoss, int minLat, int maxLat)
    : rng(chrono::steady_clock::now().time_since_epoch().count()),
      noiseProbability(noise),
      packetLossProbability(packetLoss),
      minLatency(minLat),
      maxLatency(maxLat),
      currentFrequency(1500.0),
      currentChannel(0){}


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

std::string CommunicationChannel::transmit(const std::string& message) {
    try {
        std::vector<double> noiseLevels = pythonIntegration.simulateNoise();
        currentFrequency = pythonIntegration.getNextFrequency(noiseLevels);
        currentChannel = static_cast<int>((currentFrequency - 1000.0) / 100.0);  // Assuming 10 channels between 1000 and 2000 Hz

        bool packetLost = isPacketLost();
        if (packetLost) {
            pythonIntegration.updateChannelPerformance(currentChannel, false);
            frequencyHistory.push_front({currentFrequency, false});
            if (frequencyHistory.size() > maxHistorySize) frequencyHistory.pop_back();
            return "";  // Packet lost, return empty string
        }

        std::string transmittedMessage = introduceNoise(message);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(getLatency()));
        
        pythonIntegration.updateChannelPerformance(currentChannel, true);
        frequencyHistory.push_front({currentFrequency, true});
        if (frequencyHistory.size() > maxHistorySize) frequencyHistory.pop_back();
        
        return transmittedMessage;
    } catch (const std::exception& e) {
        std::cerr << "Error during transmission: " << e.what() << std::endl;
        return "";  // Return empty string to indicate transmission failure
    }
}

    std::string CommunicationChannel::getFrequencyHoppingStats() const {
    std::ostringstream oss;
    oss << "Frequency hopping history (recent first):\n";
    for (const auto& [freq, success] : frequencyHistory) {
        oss << std::fixed << std::setprecision(2) << freq << " Hz: "
            << (success ? "✓" : "✗") << "\n";
    }
    return oss.str();
}




