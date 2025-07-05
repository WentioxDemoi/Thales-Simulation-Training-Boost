#include "Core/Core.hpp"
#include "Network/Network.hpp"
#include "UI/UI.hpp"
#include <iostream>
#include <csignal>
#include <atomic>
#include <algorithm>
#include <thread>

std::atomic<bool> g_should_exit(false);

void signalHandler(int signum) {
    std::cout << "\nSignal reçu (" << signum << "). Arrêt en cours..." << std::endl;
    g_should_exit = true;
}

void init_callbacks() {
    auto& messageManager = MessageManager::getInstance();
    auto& networkManager = NetworkManager::getInstance();
    auto& uiManager = UIManager::getInstance();
    
    networkManager.setMessageCallback([&messageManager](const std::string& message) {
        messageManager.addMessage(message);
        if (message == "__SERVER_SHUTDOWN__") {
            std::thread([&]() {
                std::this_thread::sleep_for(std::chrono::seconds(10));
                g_should_exit = true;
            }).detach();
        }
    });
    
    networkManager.setClearCallback([&messageManager]() {
        messageManager.clearMessages();
    });
    
    uiManager.setSendCallback([&networkManager](const std::string& message) {
        if (networkManager.isConnected()) {
            networkManager.sendMessage(message);
        } else {
            std::cerr << "Non connecté au serveur" << std::endl;
        }
    });

    uiManager.setMyMessageCallback([&messageManager](const std::string& message) {
        std::cout << "[Envoyé] " << message << std::endl;
        messageManager.addMessage(message);
    });
}

int main() {
    try {
        auto& messageManager = MessageManager::getInstance();
        auto& networkManager = NetworkManager::getInstance();
        auto& uiManager = UIManager::getInstance();
        
        if (!uiManager.initialize("assets/roboto/Roboto-Medium.ttf")) {
            std::cerr << "Error UI initialization" << std::endl;
            return 1;
        }
        
        init_callbacks();
        
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);
        std::signal(SIGABRT, signalHandler);
        
        std::cout << "Client running" << std::endl;
        
        if (!networkManager.connect("127.0.0.1", "8080")) {
            std::cerr << "Impossible to connect to the server" << std::endl;
            return 1;
        }
        
        while (uiManager.isOpen() && networkManager.isConnected() && !g_should_exit) {
            auto messages = messageManager.getMessages();
            uiManager.update(messages);
        }
        
        std::cout << "Cleaning..." << std::endl;
        
        if (networkManager.isConnected()) {
            networkManager.disconnect();
        }
        
        if (uiManager.isOpen()) {
            uiManager.close();
        }
        
        std::cout << "Closed" << std::endl;
        
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}