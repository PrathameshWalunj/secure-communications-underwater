#include "Submarine.h"
#include<iostream>

Submarine::Submarine(const std::string& submarineName, CommunicationChannel* commChannel)
    : name(submarineName), channel(commChannel) {}

void Submarine::sendMessage(const std::string& recipient, const std::string& subject, const std::string& content) {
    std::string message = composer.composeMessage(recipient, subject, content);
    std::string encrypted = composer.encryptMessage(message);
    fracturedMessage.fracture(encrypted, 5);  // Fracture into 5 parts

    std::cout << "Original encrypted message: " << encrypted << std::endl;

    for (const auto& fragment : fracturedMessage.getFragments()) {
        std::string transmittedFragment = channel->transmit(std::get<1>(fragment));
        if (transmittedFragment.empty()) {
            std::cout << "Fragment " << std::get<0>(fragment) << " lost during transmission." << std::endl;
        } else {
            std::cout << "Fragment " << std::get<0>(fragment) << " transmitted successfully. CRC: " << std::get<2>(fragment) << std::endl;
        }
    }
}

std::string Submarine::receiveMessage(const std::vector<std::tuple<int, std::string, uint32_t>>& receivedFragments) {
    FracturedMessage receivedMessage;
    for (const auto& fragment : receivedFragments) {
        if (!receivedMessage.verifyFragment(std::get<0>(fragment), std::get<1>(fragment), std::get<2>(fragment))) {
            std::cout << "Fragment " << std::get<0>(fragment) << " failed verification. CRC mismatch." << std::endl;
        } else {
            receivedMessage.addFragment(std::get<0>(fragment), std::get<1>(fragment), std::get<2>(fragment));
        }
    }

    if (receivedMessage.isComplete()) {
        std::string reassembled = receivedMessage.reassemble();
        std::cout << "Reassembled encrypted message: " << reassembled << std::endl;
        return composer.decryptMessage(reassembled);
    } else {
        throw std::runtime_error("Received message is incomplete");
    }
}