#include "Submarine.h"
#include "CommunicationChannel.h"
#include <iostream>
#include <string>


using namespace std;

void printHelp() {
    cout << "Available commands:\n"
         << "  send <recipient> <subject> <message> - Send a message\n"
         << "  receive - Simulate receiving a message\n"
         << "  retransmit - Retransmit lost fragments\n"
         << "  help - Show this help message\n"
         << "  exit - Exit the program\n";
}

int main() {
    CommunicationChannel channel(0.01, 0.05, 10, 100);
    Submarine submarine("Alpha", &channel);

    cout << "Secure Underwater Communication System\n";
    cout << "Type 'help' for available commands.\n";

    string command;

    while (true) {
        cout << "> ";
        cin >> command;

        if (command == "send") {
            string recipient, subject, message;
            cin >> recipient >> subject;
            cin.ignore();
            getline(cin, message);
            bool success = submarine.sendMessage(recipient, subject, message);
            cout << "Current frequency: " << channel.getCurrentFrequency() << " Hz\n";
            if (!success) {
                cout << "Some fragments were lost. You may need to retransmit.\n";
            }
        } else if (command == "receive") {
            try {
                string decryptedMessage = submarine.receiveMessage();
                cout << "Received message: " << decryptedMessage << endl;
            } catch (const exception& e) {
                cout << "Error receiving message: " << e.what() << endl;
            }
        } else if (command == "retransmit") {
            bool success = submarine.retransmitLostFragments();
            if (success) {
                cout << "All fragments retransmitted successfully.\n";
            } else {
                cout << "Some fragments were lost during retransmission. You may need to try again.\n";
            }
        } else if (command == "help") {
            printHelp();
        } else if (command == "exit") {
            break;
        } else {
            cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }

    return 0;
}








