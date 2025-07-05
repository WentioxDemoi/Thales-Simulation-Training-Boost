#ifndef NETWORKSESSION_HPP
#define NETWORKSESSION_HPP

#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::tcp;

class NetworkSession {
public:
    NetworkSession(tcp::socket socket);
    ~NetworkSession();
    void start();
    void deliver(const std::string& message);

private:
    void doRead();
    tcp::socket socket_;
    static constexpr int max_length = 1024;
    char data_[max_length];
};

#endif
