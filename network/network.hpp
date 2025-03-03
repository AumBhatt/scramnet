#include <string>
#include <vector>

#include <netdb.h>


class Socket {

    struct addrinfo hints, *res;
    std::string hostname, port_or_service;
    int fd;

    void printIPAddress();
    std::vector<struct addrinfo> setHostAddress(std::string hostname, std::string port_or_service);

    public:

    Socket(std::string hostname, std::string port_or_service);
    std::string request(std::string hostname);
    ~Socket();
};
