#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "../include/config.h"

Config::Config(std::string configPath){

    std::ifstream file(configPath, std::ios::in);
    std::stringstream fl;
    if (file.is_open()) {
        fl << file.rdbuf();
    }

    parse(fl.str());
    file.close();
}

void Config::parse (std::string buffer){
    std::vector<std::string> strings;
    std::string s, w;    
    std::size_t pos;
    std::unordered_map<std::string, std::string> vars ;
    while (true){
        pos = buffer.find('\n');
        if (pos == std::string::npos){
            break;
        }
        std::string str = buffer.substr(0, pos);
        std::size_t spacePos = str.find(' ');

        vars[buffer.substr(0, spacePos)] = buffer.substr(spacePos + 1, pos - spacePos - 1);
        buffer = buffer.substr(pos+1, buffer.length());

    }
    
    threadQuantity = atoi((vars["THREADQUANTITY"]).c_str());
    documentRoot = vars["DOCUMENT_ROOT"];
    documentRoot = documentRoot.substr(1, documentRoot.length() - 2);

    port = atoi((vars["PORT"]).c_str());
};
