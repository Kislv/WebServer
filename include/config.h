#include <string>


#define CONFIGPATH = "../config.conf"

class Config {
public:
    unsigned int threadQuantity = 8;
    std::string documentRoot =  "../";
    Config(std::string configPath);
private:
    void parse (std::string buffer) const;
}
