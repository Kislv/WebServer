#include <fstream>
#include <filesystem>
#include <iostream>
#include "../include/config.h"

Config::Config(std::string configPath){

    std::ifstream file(configPath, std::ios::in);
    std::stringstream fl;
    if (file.is_open()) {
        fl << file.rdbuf();
    }

    file.close();
    std::cout<<fl.str();
// return fl.str();
}
udo
void Config::parse (std::string buffer) const{
    // TODO complete config file parsing
};
