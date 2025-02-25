#include <iostream>
#include "network/network.hpp"
#include "url/url.hpp"

int main(int argc, char *argv[]) {
    if(argc <= 1) {
        std::cout << "Missing arguments" << "\nUsage:" << "\n ./url <website_url>" << std::endl;
        return -1;
    }
    URL u(argv[1]);
    Socket s(u.hostname, u.scheme);
    s.sendMessage();
    return 0;
}
