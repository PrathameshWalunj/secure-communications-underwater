#include "MessageComposer.h"
#include <iostream>

using namespace std;

int main() {
    // Create an instance of MessageComposer
    MessageComposer composer;

    // Compose a message
    string recipient = "BinaryBlackBelt on War";
    string subject = "Test Message";
    string content = "Hello, this is a test message!";
    string message = composer.composeMessage(recipient, subject, content);

    cout << "Composed message:" << endl;
    cout << message << endl;

    // Encrypt the message
    string encryptedMessage = composer.encryptMessage(message);

    cout << "Encrypted message:" << endl;
    cout << encryptedMessage << endl;

    // Send the encrypted message
    composer.sendMessage(encryptedMessage);

    return 0;
}
