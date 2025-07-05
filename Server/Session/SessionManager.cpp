#include "SessionManager.hpp"
#include "NetworkSession.hpp"

void SessionManager::addSession(NetworkSession* session) {
    sessions_.push_back(session);
}

void SessionManager::removeSession(NetworkSession* session) {
    sessions_.erase(std::remove(sessions_.begin(), sessions_.end(), session), sessions_.end());
    std::cout << "[Serveur] Client disconnected, active session : " << sessions_.size() << std::endl;
}

void SessionManager::broadcastMessage(const std::string& message, NetworkSession* sender) {
    if (message == "client: w") {
        for (auto session : sessions_) {
            if (session == sender) {
                session->deliver("there is " + std::to_string(sessions_.size()) + " active sessions.");
                return;
            }
        }
    }
    for (auto session : sessions_) {
        if (session != sender) {
            session->deliver(message);
        }
    }
}

void SessionManager::broadcastToAll(const std::string& message) {
    for (auto session : sessions_) {
        session->deliver(message);
    }
}
