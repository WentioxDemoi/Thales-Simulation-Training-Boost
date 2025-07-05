#include "NetworkServer.hpp"
#include "../Session/NetworkSession.hpp"
#include "../Session/SessionManager.hpp"
#include <iostream>

void NetworkServer::startServer(boost::asio::io_context& io_context, short port) {
    io_context_ = &io_context;
    acceptor_ = new tcp::acceptor(io_context, tcp::endpoint(tcp::v4(), port));
    doAccept();
}

void NetworkServer::stopServer() {
    if (acceptor_) {
        acceptor_->close();
        delete acceptor_;
        acceptor_ = nullptr;
    }
}

void NetworkServer::doAccept() {
    if (acceptor_) {
        acceptor_->async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::cout << "[Serveur] New client connected : "
                              << socket.remote_endpoint().address().to_string() << ":"
                              << socket.remote_endpoint().port() << std::endl;
                    NetworkSession* session = new NetworkSession(std::move(socket));
                    session->start();
                }
                doAccept();
            });
    }
}
