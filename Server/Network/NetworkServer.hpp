#ifndef NETWORKSERVER_HPP
#define NETWORKSERVER_HPP

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class NetworkServer {
public:
    static NetworkServer& getInstance() {
        static NetworkServer instance;
        return instance;
    }
    NetworkServer(const NetworkServer&) = delete;
    NetworkServer& operator=(const NetworkServer&) = delete;
    void startServer(boost::asio::io_context& io_context, short port);
    void stopServer();

private:
    NetworkServer() = default;
    void doAccept();
    tcp::acceptor* acceptor_;
    boost::asio::io_context* io_context_;
};

#endif
