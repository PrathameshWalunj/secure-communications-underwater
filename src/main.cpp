#include "Submarine.h"
#include "CommunicationChannel.h"
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

using namespace std;

vector<unique_ptr<Submarine>> submarines;

void transferFragments(Submarine* sender, Submarine* receiver) {
    for (const auto& [index, fragmentData] : sender->getLastSentFragments()) {
        receiver->addReceivedFragment(sender->getName(), index, get<0>(fragmentData), get<1>(fragmentData));
    }
}

void setupFragmentSendCallbacks() {
    for (auto& sub : submarines) {
        sub->setFragmentSendCallback([](const string& recipient, int index, const string& fragmentData, uint32_t crc) {
            auto recipientSub = find_if(submarines.begin(), submarines.end(),
                [&recipient](const auto& sub) { return sub->getName() == recipient; });
            if (recipientSub != submarines.end()) {
                (*recipientSub)->addReceivedFragment(recipient, index, fragmentData, crc);
            }
        });
    }
}

void printHelp() {
    cout << "Available commands:\n"
         << "  create <submarine_name> - Create a new submarine\n"
         << "  authenticate <submarine1> <submarine2> - Authenticate a session between two submarines\n"
         << "  send <sender> <recipient> <subject> <message> - Send a message\n"
         << "  receive <recipient> <sender> - Receive a message\n"
         << "  retransmit <submarine_name> - Retransmit lost fragments\n"
         << "  stats - Display frequency hopping statistics\n"
         << "  help - Show this help message\n"
         << "  exit - Exit the program\n";
}

int main() {
    CommunicationChannel channel(0.01, 0.05, 10, 100);
    vector<unique_ptr<Submarine>> submarines;

    cout << "Secure Underwater Communication System\n";
    cout << "Type 'help' for available commands.\n";

    string command;
    while (true) {
        cout << "> ";
        cin >> command;

        if (command == "create") {
            string name;
            cin >> name;
            submarines.push_back(make_unique<Submarine>(name, &channel));
            setupFragmentSendCallbacks();
            cout << "Created submarine: " << name << endl;
        } else if (command == "authenticate") {
            string name1, name2;
            cin >> name1 >> name2;
            auto sub1 = find_if(submarines.begin(), submarines.end(), 
                                [&name1](const auto& sub) { return sub->getName() == name1; });
            auto sub2 = find_if(submarines.begin(), submarines.end(), 
                                [&name2](const auto& sub) { return sub->getName() == name2; });
            if (sub1 != submarines.end() && sub2 != submarines.end()) {
                bool success = (*sub1)->authenticateSession(*(*sub2));
                cout << "Authentication " << (success ? "successful" : "failed") << endl;
            } else {
                cout << "One or both submarines not found." << endl;
            }
        } else if (command == "send") {
            string senderName, recipient, subject, message;
            cin >> senderName >> recipient >> subject;
            cin.ignore();
            getline(cin, message);
        
            auto senderSub = find_if(submarines.begin(), submarines.end(), 
                                     [&senderName](const auto& sub) { return sub->getName() == senderName; });
            auto recipientSub = find_if(submarines.begin(), submarines.end(), 
                                        [&recipient](const auto& sub) { return sub->getName() == recipient; });
        
            if (senderSub != submarines.end() && recipientSub != submarines.end()) {
                bool success = (*senderSub)->sendMessage(recipient, subject, message, true);
                cout << "Message sent " << (success ? "successfully" : "with some fragments lost") << endl;
            
                // Transfer fragments
                transferFragments(senderSub->get(), recipientSub->get());
            } else {
                cout << "Sender or recipient submarine not found." << endl;
            }
        }  else if (command == "receive") {
    string recipient, sender;
    cin >> recipient >> sender;
    auto recipientSub = find_if(submarines.begin(), submarines.end(),
        [&recipient](const auto& sub) { return sub->getName() == recipient; });
    if (recipientSub != submarines.end()) {
        string message = (*recipientSub)->receiveMessage(sender, true);
        if (!message.empty()) {
            cout << "Received message: " << message << endl;
        }
    } else {
        cout << "Recipient submarine not found." << endl;
    }
} else if (command == "retransmit") {
    string name;
    cin >> name;
    auto sub = find_if(submarines.begin(), submarines.end(),
        [&name](const auto& sub) { return sub->getName() == name; });
    if (sub != submarines.end()) {
        bool success = (*sub)->retransmitLostFragments();
        cout << (success ? "All fragments retransmitted successfully." 
                         : "Some fragments were lost during retransmission.") << endl;
    } else {
        cout << "Submarine not found." << endl;
    }
    } else if (command == "stats") {
            cout << channel.getFrequencyHoppingStats();
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
