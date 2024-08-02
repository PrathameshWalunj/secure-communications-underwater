#include "MessageComposer.h"
#include "FracturedMessage.h"
#include "CommunicationChannel.h"
#include <iostream>
#include <stdexcept>
#include <map>

using namespace std;

int main() {
    try {
        cout << "Secure Underwater Communication System" << endl;

        MessageComposer composer;
        FracturedMessage fractured;
        CommunicationChannel channel(0.01, 0.05, 10, 100);

        string message = composer.composeMessage("Submarine Alpha", "Test", "This is a test message.");
        cout << "Composed message: " << message << endl;

        string encrypted = composer.encryptMessage(message);
        cout << "Encrypted message: " << encrypted << endl;

        fractured.fracture(encrypted, 5);
        cout << "Fractured into " << fractured.getFragments().size() << " parts." << endl;

        map<int, pair<string, uint32_t>> receivedFragments;
        const int maxAttempts = 3;

        for (int attempt = 1; attempt <= maxAttempts; ++attempt) {
            cout << "Transmission attempt " << attempt << ":" << endl;

            for (const auto& fragment : fractured.getFragments()) {
                int index = get<0>(fragment);
                if (receivedFragments.find(index) == receivedFragments.end()) {
                    string transmittedFragment = channel.transmit(get<1>(fragment));
                    if (!transmittedFragment.empty()) {
                        receivedFragments[index] = make_pair(transmittedFragment, get<2>(fragment));
                        cout << "  Fragment " << index << " received." << endl;
                    } else {
                        cout << "  Fragment " << index << " lost." << endl;
                    }
                }
            }

            if (receivedFragments.size() == fractured.getFragments().size()) {
                break;
            }

            cout << "Received " << receivedFragments.size() << " out of " << fractured.getFragments().size() << " fragments." << endl;

            if (attempt < maxAttempts) {
                cout << "Attempting retransmission of lost fragments..." << endl;
            }
        }

        if (receivedFragments.size() == fractured.getFragments().size()) {
            FracturedMessage receivedFractured;
            bool allFragmentsValid = true;
            for (const auto& fragment : receivedFragments) {
                if (receivedFractured.verifyFragment(fragment.first, fragment.second.first, fragment.second.second)) {
                    receivedFractured.addFragment(fragment.first, fragment.second.first, fragment.second.second);
                } else {
                    cout << "Fragment " << fragment.first << " failed checksum verification." << endl;
                    allFragmentsValid = false;
                }
            }

            if (allFragmentsValid) {
                string reassembled = receivedFractured.reassemble();
                cout << "Reassembled message: " << reassembled << endl;

                try {
                    string decrypted = composer.decryptMessage(reassembled);
                    cout << "Decrypted message: " << decrypted << endl;
                } catch (const exception& e) {
                    cerr << "Failed to decrypt message: " << e.what() << endl;
                }
            } else {
                cout << "Failed to reassemble message: Some fragments were corrupted during transmission." << endl;
            }
        } else {
            cout << "Failed to reassemble message: Some fragments were lost during transmission after " << maxAttempts << " attempts." << endl;
        }

    } catch (const exception& e) {
        cerr << "An error occurred: " << e.what() << endl;
        return 1;
    }

    return 0;
}