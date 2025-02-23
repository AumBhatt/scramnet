/*
    Ref: https://beej.us/guide/bgnet/html/#client-server-background
*/

// standard library
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <vector>

// networking
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>


class Socket {

    struct addrinfo hints, *res;
    std::string hostname, port_or_scheme;

    void printIPAddress() {
        if(res == nullptr) {
            std::cout << "Error: No host set. Call setHostAddress to set the host.";
            return;
        }

        struct addrinfo *p;
        void *addr_ptr;
        std::string ip_ver;
        char ip_human_readable[INET6_ADDRSTRLEN];

        std::cout << "IP Addresses for " << this -> hostname << ":\n";

        for(p = res; p != nullptr; p = p -> ai_next) {
            if(p -> ai_family == AF_INET) {
                ip_ver = "IPV4";
                // pointer to location of IPV4 addr
                struct sockaddr_in *ipv4 = (struct sockaddr_in *)p -> ai_addr;
                addr_ptr = &(ipv4 -> sin_addr);
            } else {
                ip_ver = "IPV6";
                // pointer to location of IPV6 addr
                struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p -> ai_addr;
                addr_ptr = &(ipv6 -> sin6_addr);
            }
            inet_ntop(p -> ai_family, addr_ptr, ip_human_readable, sizeof ip_human_readable);

            std::cout << ip_ver << " " << std::string(ip_human_readable) << std::endl;
        }
        freeaddrinfo(p);
    }

    // retrieves all available addresses for provided hostname and port/scheme
    std::vector<struct addrinfo> setHostAddress(std::string hostname, std::string port_or_scheme) {

        std::vector<struct addrinfo> res_vector;
        struct addrinfo *res_ptr;

        int status;
        const char *cstr_hostname = hostname.c_str();
        const char *cstr_port_or_scheme = port_or_scheme != "" ? port_or_scheme.c_str() : nullptr;
        this -> hostname = hostname;
        this -> port_or_scheme = port_or_scheme;

        std::memset(&hints, 0, sizeof hints);   // make sure the struct is empty
        hints.ai_family = AF_UNSPEC;            // use either AF_INET (IPV4) / AF_INET6 (IPV6)
        hints.ai_socktype = SOCK_STREAM;        // use TCP sockets

        status = getaddrinfo(cstr_hostname, cstr_port_or_scheme, &hints, &res);

        if(status != 0) {
            std::cout << "Error in getaddrinfo:" << gai_strerror(status) << std::endl;
            exit(1);
        }

        for(res_ptr = res; res_ptr != nullptr; res_ptr = res_ptr -> ai_next) {
            res_vector.push_back(*res_ptr);
        }

        return res_vector;
    }

    public:
    Socket(std::string hostname, std::string port_or_scheme) {
        this -> setHostAddress(hostname, port_or_scheme);
        this -> printIPAddress();
    }

    ~Socket() {
        freeaddrinfo(this -> res);
    }
};

int main(int argc, char *argv[]) {
    if(argc != 3) {
        std::cout << "Usage: network <hostname> <port/scheme>\n";
        exit(1);
    }


    Socket s(argv[1], argv[2]);
    return 0;
}
