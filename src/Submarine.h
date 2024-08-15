#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "MessageComposer.h"
#include "FracturedMessage.h"
#include "CommunicationChannel.h"
#include "ZeroKnowledgeAuth.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include<functional>

class Submarine {
private:
    std::string name;
    MessageComposer composer;
    FracturedMessage fracturedMessage;
    CommunicationChannel* channel;
    std::map<int, std::tuple<std::string, uint32_t>> lastSentFragments;
    ZeroKnowledgeAuth zkAuth;
    std::unordered_map<std::string, bool> authenticatedSessions;
    std::map<std::string, std::map<int, std::tuple<std::string, uint32_t>>> receivedFragments;
    std::string lastRecipient;  // To keep track of the last message recipient for retransmission
    std::function<void(const std::string&, int, const std::string&, uint32_t)> fragmentSendCallback;
    std::unordered_map<std::string, std::string> rsaPublicKeys;

public:
    Submarine(const std::string& submarineName, CommunicationChannel* commChannel);
    
    bool retransmitLostFragments();
    bool authenticateTo(Submarine& other);
    bool verifyAuthentication(Submarine& other, int commitment, int response, const std::vector<bool>& challenge);
    std::vector<int> getZKPublicKey() const { return zkAuth.getPublicKey(); }
    std::string getName() const { return name; }
    bool sendAuthenticatedMessage(Submarine& receiver, const std::string& subject, const std::string& content);
    std::string receiveAuthenticatedMessage(Submarine& sender);
    bool isAuthenticated(const std::string& otherName) const;
    bool authenticateSession(Submarine& other);
    bool sendMessage(const std::string& recipient, const std::string& subject, const std::string& content, bool requireAuth = false);
    std::string receiveMessage(const std::string& sender, bool requireAuth = false);
    bool sendFragment(const std::string& recipient, int index, const std::string& fragmentData, uint32_t crc);
    void addReceivedFragment(const std::string& sender, int index, const std::string& fragmentData, uint32_t crc);
    const std::map<int, std::tuple<std::string, uint32_t>>& getLastSentFragments() const { return lastSentFragments; }
    void setFragmentSendCallback(std::function<void(const std::string&, int, const std::string&, uint32_t)> callback);
    void addRSAPublicKey(const std::string& submarine_name, const std::string& public_key);
    std::string getRSAPublicKey() const;
    bool secureConnect(Submarine& other);

};

#endif 