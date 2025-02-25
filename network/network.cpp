/*
    Ref: https://beej.us/guide/bgnet/html/#client-server-background
*/

// standard library
#include <cerrno>
#include <cstdio>
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

#include "network.hpp"

void Socket::printIPAddress() {
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
            ip_ver = "IPv4";
            // pointer to location of IPv4 addr
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p -> ai_addr;
            addr_ptr = &(ipv4 -> sin_addr);
        } else {
            ip_ver = "IPv6";
            // pointer to location of IPv6 addr
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p -> ai_addr;
            addr_ptr = &(ipv6 -> sin6_addr);
        }
        inet_ntop(p -> ai_family, addr_ptr, ip_human_readable, sizeof ip_human_readable);

        std::cout << ip_ver << " " << std::string(ip_human_readable) << std::endl;
    }
    freeaddrinfo(p);
}

// retrieves all available addresses for provided hostname and port/scheme
std::vector<struct addrinfo> Socket::setHostAddress(std::string hostname, std::string port_or_service) {

    std::vector<struct addrinfo> res_vector;
    struct addrinfo *res_ptr;

    int status;
    const char *cstr_hostname = hostname.c_str();
    const char *cstr_port_or_service = port_or_service != "" ? port_or_service.c_str() : nullptr;
    this -> hostname = hostname;
    this -> port_or_service = port_or_service;

    std::memset(&hints, 0, sizeof hints);   // make sure the struct is empty
    hints.ai_family = AF_UNSPEC;            // use either AF_INET (IPv4) / AF_INET6 (IPv6)
    hints.ai_socktype = SOCK_STREAM;        // use TCP sockets

    status = getaddrinfo(cstr_hostname, cstr_port_or_service, &hints, &res);

    if(status != 0) {
        std::cout << "Error in getaddrinfo:" << gai_strerror(status) << std::endl;
        exit(1);
    }

    for(res_ptr = res; res_ptr != nullptr; res_ptr = res_ptr -> ai_next) {
        res_vector.push_back(*res_ptr);
    }

    return res_vector;
}

Socket::Socket(std::string hostname, std::string port_or_service) {
    this -> setHostAddress(hostname, port_or_service);
    // this -> printIPAddress();
    int sockfd = socket(this -> res -> ai_family, this -> res -> ai_socktype, this -> res -> ai_protocol);
    int status = connect(sockfd, this -> res -> ai_addr, this -> res -> ai_addrlen);

    if(status == -1) {
        std::cout << "Socket: error connecting. " << errno << std::endl;
        exit(1);
    }

    this -> fd = sockfd;
}

void Socket::sendMessage() {
    std::string msg =
        "GET /index.html HTTP/1.0\r\n"
        "Host: example.org\r\n"
        "\r\n";
    char response[4096];
    send(this -> fd, msg.c_str(), msg.length(), 0);
    int bytesRead = recv(this -> fd, response, 4095, 0);
    if(bytesRead == -1) {
        perror("recv");
        exit(1);
    }
    std::cout << "Bytes read: " << bytesRead << "\nResponse: " << std::string(response) << std::endl;
}

Socket::~Socket() {
    // close(this -> fd);
    freeaddrinfo(this -> res);
}
