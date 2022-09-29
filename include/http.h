#pragma once

#include <string>
#include <iostream>
#include <vector>

enum class Method
{
    GET,
    HEAD
};

class Request {
public:
    std::string method;
    // Method method;
    std::string url;
    std::string protocol;
    std::vector<std::pair<std::string, std::string>> headers;
    std::vector<char> content;
    void parse(std::string request);
    void methodToEnum(std::string str);
    void print ();
    std::string buildFilePath();

};

class Response {
public:
    std::string protocol = "HTTP/1.1";
    int status = 200;
    std::string explanation = "OK";
    std::string buildHeaders(int contentLength, std::string url) const;
    std::string build(Request request);
private:
    std::string defineContentType(std::string url) const;
};
