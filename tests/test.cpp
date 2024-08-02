#include "../src/MessageComposer.h"
#include "../src/FracturedMessage.h"
#include "../src/CommunicationChannel.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

void test_message_composer() {
    MessageComposer composer;
    
    // Test message composition
    string recipient = "Submarine Alpha";
    string subject = "Test Transmission";
    string content = "This is a secure test message.";
    string message = composer.composeMessage(recipient, subject, content);
    
    assert(message.find("To: Submarine Alpha") != string::npos);
    assert(message.find("Subject: Test Transmission") != string::npos);
    assert(message.find("This is a secure test message.") != string::npos);
    
    // Test message encryption and decryption
    string encryptedMessage = composer.encryptMessage(message);
    assert(encryptedMessage != message);
    
    string decryptedMessage = composer.decryptMessage(encryptedMessage);
    assert(decryptedMessage == message);
    
    cout << "MessageComposer tests passed." << endl;
}

void test_fractured_message() {
    FracturedMessage fm;
    string original = "This is a test message for fracturing and reassembly.";
    
    // Test fracturing
    fm.fracture(original, 5);
    auto fragments = fm.getFragments();
    
    assert(fragments.size() == 5);
    
    cout << "Original message: " << original << endl;
    cout << "Fractured into " << fragments.size() << " parts:" << endl;
    for (const auto& fragment : fragments) {
        cout << "  " << get<0>(fragment) << ": " << get<1>(fragment) << endl;
    }
    
    // Test reassembly
    string reassembled = fm.reassemble();
    cout << "Reassembled message: " << reassembled << endl;
    
    assert(reassembled == original);
    
    // Test with different number of fragments
    fm.fracture(original, 10);
    fragments = fm.getFragments();
    assert(fragments.size() == 10);
    reassembled = fm.reassemble();
    assert(reassembled == original);
    
    // Test adding fragments manually
    FracturedMessage fm2;
    for (const auto& fragment : fragments) {
        fm2.addFragment(get<0>(fragment), get<1>(fragment), get<2>(fragment));
    }
    reassembled = fm2.reassemble();
    assert(reassembled == original);
    
    // Test incomplete message
    FracturedMessage fm3;
    for (size_t i = 0; i < fragments.size() - 1; ++i) {
        fm3.addFragment(get<0>(fragments[i]), get<1>(fragments[i]), get<2>(fragments[i]));
    }
    assert(!fm3.isComplete());
    
    try {
        fm3.reassemble();
        assert(false); // This should not be reached
    } catch (const runtime_error& e) {
        // Expected exception
    }
    
    // Test corrupted fragment
    FracturedMessage fm4;
    for (const auto& fragment : fragments) {
        if (get<0>(fragment) == 0) {
            // Corrupt the first fragment
            fm4.addFragment(0, get<1>(fragment) + "corrupted", get<2>(fragment));
        } else {
            fm4.addFragment(get<0>(fragment), get<1>(fragment), get<2>(fragment));
        }
    }
    
    try {
        fm4.reassemble();
        assert(false); // This should not be reached
    } catch (const runtime_error& e) {
        // Expected exception
    }
    
    cout << "FracturedMessage tests passed." << endl;
}

void test_communication_channel() {
    cout << "Starting CommunicationChannel tests..." << endl;
    
    CommunicationChannel channel(0.01, 0.05, 10, 100);
    
    string originalMessage = "This is a test message for the communication channel.";
    
    // Test multiple transmissions
    int successfulTransmissions = 0;
    int noisyTransmissions = 0;
    int totalTransmissions = 1000;
    
    auto start = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < totalTransmissions; ++i) {
        string receivedMessage = channel.transmit(originalMessage);
        
        if (!receivedMessage.empty()) {
            successfulTransmissions++;
            if (receivedMessage != originalMessage) {
                noisyTransmissions++;
            }
        }
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    cout << "Communication Channel Test Results:" << endl;
    cout << "Total transmissions: " << totalTransmissions << endl;
    cout << "Successful transmissions: " << successfulTransmissions << endl;
    cout << "Noisy transmissions: " << noisyTransmissions << endl;
    cout << "Packet loss rate: " << (totalTransmissions - successfulTransmissions) * 100.0 / totalTransmissions << "%" << endl;
    cout << "Noise rate: " << noisyTransmissions * 100.0 / successfulTransmissions << "%" << endl;
    cout << "Average latency: " << duration.count() / totalTransmissions << " ms" << endl;
    
    assert(successfulTransmissions > 0 && successfulTransmissions < totalTransmissions);
    assert(noisyTransmissions > 0 && noisyTransmissions < successfulTransmissions);
    
    cout << "CommunicationChannel tests passed." << endl;
}

int main() {
    try {
        test_message_composer();
        test_fractured_message();
        test_communication_channel();
        
        cout << "All tests passed successfully." << endl;
    } catch (const exception& e) {
        cerr << "An error occurred during testing: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}