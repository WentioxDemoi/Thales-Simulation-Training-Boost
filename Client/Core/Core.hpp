#ifndef CORE_HPP
#define CORE_HPP
#include <string>
#include <deque>
#include <mutex>
#include <vector>

class MessageManager {
public:
    static MessageManager& getInstance() {
        static MessageManager instance;
        return instance;
    }
    
    MessageManager(const MessageManager&) = delete;
    MessageManager& operator=(const MessageManager&) = delete;
    
    void addMessage(const std::string& message);
    std::vector<std::string> getMessages() const;
    void clearMessages();
    size_t getMessageCount() const;
    
private:
    MessageManager() : max_messages(20) {}
    ~MessageManager() = default;
    
    std::deque<std::string> messages;
    mutable std::mutex messages_mutex;
    size_t max_messages;
};

#endif