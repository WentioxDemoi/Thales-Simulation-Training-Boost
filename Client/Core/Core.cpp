#include "Core.hpp"
#include <iostream>

void MessageManager::addMessage(const std::string& message) {
    std::lock_guard<std::mutex> lock(messages_mutex);
    messages.push_back(message);
    if (messages.size() > max_messages) {
        messages.pop_front();
    }
    std::cout << "[Received] " << message << std::endl;
}

std::vector<std::string> MessageManager::getMessages() const {
    std::lock_guard<std::mutex> lock(messages_mutex);
    return std::vector<std::string>(messages.begin(), messages.end());
}

void MessageManager::clearMessages() {
    std::lock_guard<std::mutex> lock(messages_mutex);
    messages.clear();
}

size_t MessageManager::getMessageCount() const {
    std::lock_guard<std::mutex> lock(messages_mutex);
    return messages.size();
}