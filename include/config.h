#include <string>


#define CONFIGPATH = "../config.conf"

class Config {
public:
    int threadQuantity = 8;
    std::string documentRoot =  "../";
    int port =  8080;
    Config(std::string configPath);
private:
    void parse (std::string buffer);
};
