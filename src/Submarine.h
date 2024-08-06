#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "MessageComposer.h"
#include "FracturedMessage.h"
#include "CommunicationChannel.h"
#include <string>
#include <vector>

class Submarine {
private:
    std::string name;
    MessageComposer composer;
    FracturedMessage fracturedMessage;
    CommunicationChannel* channel;

public:
    Submarine(const std::string& submarineName, CommunicationChannel* commChannel);
    void sendMessage(const std::string& recipient, const std::string& subject, const std::string& content);
    std::string receiveMessage(const std::vector<std::tuple<int, std::string, uint32_t>>& receivedFragments);
    const std::vector<std::tuple<int, std::string, uint32_t>> getLastSentFragments() const {
        return fracturedMessage.getFragments();
    }
};

#endif