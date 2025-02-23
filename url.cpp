#include <algorithm>
#include <iostream>
#include <vector>

#define SCHEME_DELIM "://"

class URL {
    private:
    std::vector<std::string> valid_hosts = std::vector<std::string>{
        "http",
        "https"
    };
    void printError(std::string message = "") {
        std::cout << "Invalid URL: " << (message != "" ? message : "unknown reason") << std::endl;
    }

    public:
    std::string scheme;
    std::string hostname;
    std::string path;

    URL(std::string url) {
        try {
            const size_t scheme_start = url.find("://");
            if(scheme_start == std::string::npos) {
                throw 100;
            }
            this -> scheme = url.substr(0, scheme_start);
            if(std::find(
                valid_hosts.begin(),
                valid_hosts.end(),
                this -> scheme
            ) == valid_hosts.end()) {
                throw 101;
            }
            url.erase(0, scheme_start + 3);

            const size_t host_tld = url.find_last_of('.');
            if(host_tld == std::string::npos) {
                throw 102;
            }
            const size_t host_end = url.find_last_of('/');

            this -> hostname = url.substr(0, host_end);
            url.erase(0, host_end);

            this -> path = url;

            std::cout << "Scheme:" << this -> scheme << "\nHostname: " << this -> hostname << "\nPath: " << (this -> path == "" ? "/" : this -> path) << std::endl;
        } catch (int error) {
            switch(error) {
                case 100: printError("missing scheme"); break;
                case 101: printError("\"" + this -> scheme + "\" scheme not supported"); break;
                case 102: printError("missing hostname"); break;
            }
        }
    }
};

int main(int argc, char *argv[]) {
    if(argc <= 1) {
        std::cout << "Missing arguments" << "\nUsage:" << "\n ./url <website_url>" << std::endl;
        return -1;
    }
    URL u(argv[1]);
    return 0;
}
