#include "Submarine.h"
#include <iostream>
#include<sstream>
#include<algorithm>
#include<exception>

using namespace std;  

Submarine::Submarine(const std::string& submarineName, CommunicationChannel* commChannel)
    : name(submarineName), channel(commChannel) {}

void Submarine::setFragmentSendCallback(std::function<void(const std::string&, int, const std::string&, uint32_t)> callback) {
    fragmentSendCallback = std::move(callback);
}

void Submarine::addRSAPublicKey(const std::string& submarine_name, const std::string& public_key) {
    rsaPublicKeys[submarine_name] = public_key;
}

std::string Submarine::getRSAPublicKey() const {
    string publicKeyStr;
    for (int key : zkAuth.getPublicKey()){
        publicKeyStr += to_string(key) + " ";
    }
    return publicKeyStr;
}

bool Submarine::secureConnect(Submarine& other) {
    //Step-1 Zero Knowledge Authentication
    if(!this->authenticateTo(other) || !other.authenticateTo(*this)) {
        cout<<"Authentication failed." << endl;
        return false;
    }

    //Step-2 Exchange RSA Public Keys
    string myRSAPublicKey = this->getRSAPublicKey();
    string otherRSAPublicKey = other.getRSAPublicKey();

    this->addRSAPublicKey(other.getName(), otherRSAPublicKey);
    other.addRSAPublicKey(this->getName(), myRSAPublicKey);

    cout << "Secure connection established between " << this->getName()
         << " and " << other.getName() << endl;
    return true;
}

bool Submarine::sendMessage(const string& recipient, const string& subject, const string& content, bool requireAuth) {
    if (requireAuth && !isAuthenticated(recipient)) {
        cout << "Authentication required. Please authenticate first." << endl;
        return false;
    }

    auto it = rsaPublicKeys.find(recipient);
    if (it == rsaPublicKeys.end()) {
        cout << "Recipient's RSA public key not found. Cannot encrypt message." << std::endl;
        return false;
    }

    string message = composer.composeMessage(recipient, subject, content);
    string encrypted = composer.encryptMessage(message, it->second);
    fracturedMessage.fracture(encrypted, 5); // Fracture into 5 parts
    
    cout << "Original encrypted message: " << encrypted << endl;
    
    lastSentFragments.clear();
    lastRecipient = recipient;
    bool allTransmitted = true;
    
    for (const auto& fragment : fracturedMessage.getFragments()) {
        int index = get<0>(fragment);
        string fragmentData = get<1>(fragment);
        uint32_t crc = get<2>(fragment);
        
        cout << "Sending fragment " << index << " - Original CRC: " << crc << endl;
        
        string transmittedFragment = channel->transmit(fragmentData);
        if (transmittedFragment.empty()) {
            cout << "Fragment " << index << " lost during transmission." << endl;
            allTransmitted = false;
        } else {
            uint32_t transmittedCRC = FracturedMessage::calculateCRC32(transmittedFragment);
            cout << "Fragment " << index << " transmitted successfully. Original CRC: " << crc << ", Transmitted CRC: " << transmittedCRC << endl;
            lastSentFragments[index] = make_tuple(transmittedFragment, transmittedCRC);
        }
    }
    
    return allTransmitted;
}




bool Submarine::sendFragment(const std::string& recipient, int index, const std::string& fragmentData, uint32_t crc) {
    std::string transmittedFragment = channel->transmit(fragmentData);
    if (!transmittedFragment.empty()) {
        std::cout << "Fragment " << index << " transmitted successfully to " << recipient << ". CRC: " << crc << std::endl;
        lastSentFragments[index] = std::make_tuple(transmittedFragment, crc);
        if (fragmentSendCallback) {
            fragmentSendCallback(recipient, index, fragmentData, crc);
        }
        return true;
    }
    std::cout << "Fragment " << index << " lost during transmission to " << recipient << "." << std::endl;
    return false;
}

void Submarine::addReceivedFragment(const std::string& sender, int index, const std::string& fragmentData, uint32_t crc) {
    receivedFragments[sender][index] = std::make_tuple(fragmentData, crc);
}

string Submarine::receiveMessage(const string& sender, bool requireAuth) {
    if (requireAuth && !isAuthenticated(sender)) {
        cout << "Authentication required with " << sender << ". Message rejected." << endl;
        return "";
    }
    
    auto& fragments = receivedFragments[sender];
    cout << "Received " << fragments.size() << " out of 5 fragments from " << sender << endl;
    
    if (fragments.size() != 5) {
        cout << "Message is incomplete. Waiting for more fragments." << endl;
        return "";
    }
    
    FracturedMessage receivedMessage;
    for (const auto& [index, fragmentData] : fragments) {
        uint32_t storedCRC = get<1>(fragmentData);
        uint32_t calculatedCRC = FracturedMessage::calculateCRC32(get<0>(fragmentData));
        cout << "Receiving fragment " << index << " - Stored CRC: " << storedCRC << ", Calculated CRC: " << calculatedCRC << endl;
        
        try {
            receivedMessage.addFragment(index, get<0>(fragmentData), storedCRC);
        } catch (const exception& e) {
            cout << "Error adding fragment " << index << ": " << e.what() << endl;
        }
    }

    try {
        string reassembled = receivedMessage.reassemble();
        cout << "Message reassembled successfully. Reassembled message: " << reassembled << endl;
        fragments.clear();  // Clear received fragments after successful reassembly
        
        // Decrypt the message
        size_t colon_pos = reassembled.find(':');
        if (colon_pos == string::npos) {
            throw runtime_error("Invalid encrypted message format");
        }

        string encrypted_key_str = reassembled.substr(0, colon_pos);
        string encrypted_message = reassembled.substr(colon_pos + 1);

        vector<unsigned char> encrypted_key = composer.base64Decode(encrypted_key_str);

        string decrypted = composer.decryptMessage(encrypted_message, encrypted_key);
        cout << "Decrypted message: " << decrypted << endl;
        return decrypted;
    } catch (const exception& e) {
        cout << "Error reassembling or decrypting message: " << e.what() << endl;
        // Print out all stored fragments for debugging
        for (const auto& [index, fragmentData] : fragments) {
            cout << "Stored fragment " << index << " - CRC: " << get<1>(fragmentData) << endl;
        }
        return "";
    }
}


bool Submarine::retransmitLostFragments() {
    if (lastSentFragments.empty()) {
        std::cout << "No fragments to retransmit." << std::endl;
        return false;
    }

    bool allRetransmitted = true;
    for (const auto& [index, fragmentData] : lastSentFragments) {
        std::string transmittedFragment = channel->transmit(std::get<0>(fragmentData));
        if (transmittedFragment.empty()) {
            std::cout << "Fragment " << index << " lost during retransmission." << std::endl;
            allRetransmitted = false;
        } else {
            std::cout << "Fragment " << index << " retransmitted successfully." << std::endl;
        }
    }

    return allRetransmitted;
}




bool Submarine::authenticateTo(Submarine& other) {
    std::cout << name << " is authenticating to " << other.getName() << std::endl;
    std::vector<bool> challenge = ZeroKnowledgeAuth::generateChallenge();
    auto [commitment, response] = zkAuth.proveIdentity(other.getZKPublicKey(), challenge);
    return other.verifyAuthentication(*this, commitment, response, challenge);
}




bool Submarine::verifyAuthentication(Submarine& other, int commitment, int response, const std::vector<bool>& challenge) {
    std::cout << name << " is verifying " << other.getName() << "'s authentication" << std::endl;
    return zkAuth.verifyIdentity(other.getZKPublicKey(), commitment, response, challenge);
}




bool Submarine::sendAuthenticatedMessage(Submarine& receiver, const std::string& subject, const std::string& content) {
    if (!this->authenticateTo(receiver)) {
        std::cout << "Authentication failed. Message not sent." << std::endl;
        return false;
    }
    
    return sendMessage(receiver.getName(), subject, content, true);
}

bool Submarine::authenticateSession(Submarine& other) {
    if (authenticateTo(other)) {
        authenticatedSessions[other.getName()] = true;
        other.authenticatedSessions[this->getName()] = true;
        return true;
    }
    return false;
}

bool Submarine::isAuthenticated(const std::string& otherName) const {
    auto it = authenticatedSessions.find(otherName);
    return it != authenticatedSessions.end() && it->second;
}


std::string Submarine::receiveAuthenticatedMessage(Submarine& sender) {
    if (!sender.authenticateTo(*this)) {
        std::cout << "Sender authentication failed. Message rejected." << std::endl;
        return "";
    }
    
    return receiveMessage(sender.getName(), true);
}



