#include "Network.hpp"
#include <iostream>

bool NetworkManager::connect(const std::string& host, const std::string& port) {
    try {
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(host, port);
        boost::asio::connect(socket, endpoints);
        connected = true;
        read_thread = std::thread(&NetworkManager::readThreadFunction, this);
        std::cout << "Connected to server" << std::endl;
        return true;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        connected = false;
        return false;
    }
}

void NetworkManager::disconnect() {
    if (connected) {
        connected = false;
        try {
            boost::asio::write(socket, boost::asio::buffer("__DISCONNECT__"));
            socket.close();
        } catch (std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        if (read_thread.joinable()) {
            read_thread.join();
        }
    }
}

bool NetworkManager::sendMessage(const std::string& message) {
    if (!connected) {
        return false;
    }
    try {
        if (message == "client: clear") {
            if (clear_callback) {
                clear_callback();
            }
            return true;
        }
        boost::asio::write(socket, boost::asio::buffer(message));
        return true;
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        connected = false;
        return false;
    }
}

void NetworkManager::setMessageCallback(std::function<void(const std::string&)> callback) {
    message_callback = callback;
}

void NetworkManager::setClearCallback(std::function<void(void)> callback) {
    clear_callback = callback;
}

bool NetworkManager::isConnected() const {
    return connected;
}

void NetworkManager::readThreadFunction() {
    try {
        char data[1024];
        while (connected) {
            boost::system::error_code error;
            size_t length = socket.read_some(boost::asio::buffer(data), error);
            if (error == boost::asio::error::eof) {
                break;
            } else if (error) {
                throw boost::system::system_error(error);
            }
            std::string msg = std::string(data, length);
            if (message_callback) {
                message_callback(msg);
            }
        }
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        connected = false;
    }
}