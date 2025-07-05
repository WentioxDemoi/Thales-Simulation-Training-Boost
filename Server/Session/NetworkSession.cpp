#include <string>
#include "NetworkSession.hpp"
#include "SessionManager.hpp"
#include <algorithm>

NetworkSession::NetworkSession(tcp::socket socket)
    : socket_(std::move(socket)) {}

NetworkSession::~NetworkSession() {
    SessionManager::getInstance().removeSession(this);
}

void NetworkSession::start() {
    SessionManager::getInstance().addSession(this);
    doRead();
}

void NetworkSession::deliver(const std::string& message) {
    boost::asio::async_write(socket_, boost::asio::buffer(message),
        [/*this*/](boost::system::error_code ec, std::size_t /*length*/) {
            if (ec) {
                std::cout << ec.message() << std::endl;
            }
        });
}

void NetworkSession::doRead() {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                std::string message(data_, length);
                message.erase(std::remove_if(message.begin(), message.end(), [](unsigned char c) {
                    return  c == '\0';
                }), message.end());
                std::cout << "[Serveur] Message received : " << message << std::endl;
                if (message != "__DISCONNECT__") {
                    SessionManager::getInstance().broadcastMessage(message, this);
                    doRead();
                } else {
                    std::cout << "[Serveur] Client disconnected" << std::endl;
                    delete this;
                    return;
                }
                
            } else {
                delete this;
            }
        });
}