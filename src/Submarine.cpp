#include "Submarine.h"
#include <iostream>

using namespace std;  

Submarine::Submarine(const std::string& submarineName, CommunicationChannel* commChannel)
    : name(submarineName), channel(commChannel) {}

bool Submarine::sendMessage(const std::string& recipient, const std::string& subject, const std::string& content) {
    string message = composer.composeMessage(recipient, subject, content);
    string encrypted = composer.encryptMessage(message);
    fracturedMessage.fracture(encrypted, 5);  // Fracture into 5 parts

    cout << "Original encrypted message: " << encrypted << endl;

    lastSentFragments.clear();
    bool allTransmitted = true;

    for (const auto& fragment : fracturedMessage.getFragments()) {
        int index = get<0>(fragment);
        string fragmentData = get<1>(fragment);
        uint32_t crc = get<2>(fragment);

        string transmittedFragment = channel->transmit(fragmentData);
        if (transmittedFragment.empty()) {
            cout << "Fragment " << index << " lost during transmission." << endl;
            allTransmitted = false;
        } else {
            cout << "Fragment " << index << " transmitted successfully. CRC: " << crc << endl;
            lastSentFragments[index] = make_tuple(fragmentData, crc);
        }
    }

    return allTransmitted;
}

string Submarine::receiveMessage() {
    if (lastSentFragments.size() != 5) {
        throw runtime_error("Received message is incomplete. Try retransmitting lost fragments.");
    }

    FracturedMessage receivedMessage;
    for (const auto& [index, fragmentData] : lastSentFragments) {
        receivedMessage.addFragment(index, get<0>(fragmentData), get<1>(fragmentData));
    }

    string reassembled = receivedMessage.reassemble();
    cout << "Reassembled encrypted message: " << reassembled << endl;
    return composer.decryptMessage(reassembled);
}

bool Submarine::retransmitLostFragments() {
    if (lastSentFragments.size() == 5) {
        cout << "All fragments already received. No retransmission needed." << endl;
        return true;
    }

    bool allRetransmitted = true;
    for (const auto& fragment : fracturedMessage.getFragments()) {
        int index = get<0>(fragment);
        if (lastSentFragments.find(index) == lastSentFragments.end()) {
            string fragmentData = get<1>(fragment);
            uint32_t crc = get<2>(fragment);
            
            string transmittedFragment = channel->transmit(fragmentData);
            if (transmittedFragment.empty()) {
                cout << "Fragment " << index << " lost during retransmission." << endl;
                allRetransmitted = false;
            } else {
                cout << "Fragment " << index << " retransmitted successfully. CRC: " << crc << endl;
                lastSentFragments[index] = make_tuple(fragmentData, crc);
            }
        }
    }

    return allRetransmitted;
}
