#include <string>
#include <vector>

class URL {
    std::vector<std::string> valid_hosts = std::vector<std::string>{
        "http",
        "https"
    };

    void printError(std::string message);

    public:
    std::string scheme;
    std::string hostname;
    std::string path;

    URL(std::string url);
};
