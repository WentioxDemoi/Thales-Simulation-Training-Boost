#ifndef SESSIONMANAGER_HPP
#define SESSIONMANAGER_HPP

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

class NetworkSession;

class SessionManager {
public:
    static SessionManager& getInstance() {
        static SessionManager instance;
        return instance;
    }
    SessionManager(const SessionManager&) = delete;
    SessionManager& operator=(const SessionManager&) = delete;
    void addSession(NetworkSession* session);
    void removeSession(NetworkSession* session);
    void broadcastMessage(const std::string& message, NetworkSession* sender);
    void broadcastToAll(const std::string& message);

private:
    SessionManager() = default;
    std::vector<NetworkSession*> sessions_;
};

#endif
