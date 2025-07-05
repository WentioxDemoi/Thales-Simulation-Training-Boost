#ifndef NETWORK_HPP
#define NETWORK_HPP
#include <boost/asio.hpp>
#include <string>
#include <thread>
#include <functional>

using boost::asio::ip::tcp;

class NetworkManager {
public:
    static NetworkManager& getInstance() {
        static NetworkManager instance;
        return instance;
    }
    
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;
    
    bool connect(const std::string& host, const std::string& port);
    void disconnect();
    bool sendMessage(const std::string& message);
    void setMessageCallback(std::function<void(const std::string&)> callback);
    void setClearCallback(std::function<void(void)> callback);
    bool isConnected() const;
    
private:
    NetworkManager() : socket(io_context), connected(false) {}
    ~NetworkManager() { disconnect(); }
    
    boost::asio::io_context io_context;
    tcp::socket socket;
    std::thread read_thread;
    bool connected;
    std::function<void(const std::string&)> message_callback;
    std::function<void(void)> clear_callback;
    
    void readThreadFunction();
};

#endif // NETWORK_HPP