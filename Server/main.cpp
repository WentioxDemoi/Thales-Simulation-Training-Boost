#include "Network/NetworkServer.hpp"
#include "Session/SessionManager.hpp"
#include <boost/asio.hpp>
#include <iostream>
#include <csignal>
#include <atomic>
#include <thread>

bool should_exit;

void signalHandler(int signum) {
    std::cout << "\nSignal received (" << signum << "). shut down..." << std::endl;
    SessionManager::getInstance().broadcastToAll("__SERVER_SHUTDOWN__");
    should_exit = true;
}

int main() {
    try {
        boost::asio::io_context io_context;
        should_exit = false;
        NetworkServer *server = &NetworkServer::getInstance();
        server->startServer(io_context, 8080);
        
        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);
        std::signal(SIGABRT, signalHandler);
        
        std::cout << "Server started on port 8080" << std::endl;
        std::cout << "Ctrl+C to stop" << std::endl;
        
        while (!should_exit) {
            io_context.poll();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        server->stopServer();
        std::cout << "Server stoped." << std::endl;
        
    } catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}