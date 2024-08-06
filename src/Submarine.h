#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "MessageComposer.h"
#include "FracturedMessage.h"
#include "CommunicationChannel.h"
#include <string>
#include <vector>
#include <map>

class Submarine {
private:
    std::string name;
    MessageComposer composer;
    FracturedMessage fracturedMessage;
    CommunicationChannel* channel;
    std::map<int, std::tuple<std::string, uint32_t>> lastSentFragments;

public:
    Submarine(const std::string& submarineName, CommunicationChannel* commChannel);
    
    bool sendMessage(const std::string& recipient, const std::string& subject, const std::string& content);
    std::string receiveMessage();
    bool retransmitLostFragments();
};

#endif 