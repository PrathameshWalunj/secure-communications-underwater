
#include "Submarine.h"
#include <iostream>
#include <string>
#include <vector>

void printHelp() {
    std::cout << "Available commands:\n"
              << "  send <recipient> <subject> <message> - Send a message\n"
              << "  receive - Simulate receiving a message\n"
              << "  help - Show this help message\n"
              << "  exit - Exit the program\n";
}

int main() {
    CommunicationChannel channel(0.01, 0.05, 10, 100);
    Submarine submarine("Alpha", &channel);

    std::cout << "Secure Underwater Communication System\n";
    std::cout << "Type 'help' for available commands.\n";

    std::string command;
    std::vector<std::tuple<int, std::string, uint32_t>> lastSentFragments;

    while (true) {
        std::cout << "> ";
        std::cin >> command;

        if (command == "send") {
            std::string recipient, subject, message;
            std::cin >> recipient >> subject;
            std::cin.ignore();
            std::getline(std::cin, message);
            submarine.sendMessage(recipient, subject, message);
            lastSentFragments = submarine.getLastSentFragments();
        } else if (command == "receive") {
            // Use the last sent fragments for simulation
            if (lastSentFragments.empty()) {
                std::cout << "No message has been sent yet. Please send a message first." << std::endl;
                continue;
            }

            try {
                std::string decryptedMessage = submarine.receiveMessage(lastSentFragments);
                std::cout << "Received message: " << decryptedMessage << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Error receiving message: " << e.what() << std::endl;
            }
        } else if (command == "help") {
            printHelp();
        } else if (command == "exit") {
            break;
        } else {
            std::cout << "Unknown command. Type 'help' for available commands.\n";
        }
    }

    return 0;
}