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
    std::string url;
    std::string protocol;
    std::vector<std::pair<std::string, std::string>> headers;
    std::vector<char> content;
    bool isIndex = false;
    void parse(std::string request);
    void methodToEnum(std::string str);
    void print ();
    std::string buildFilePath();
    void percentDecode();
private:
    char toHex(char c);
};

class Response {
public:
    std::string protocol = "HTTP/1.1";
    int status = 200;
    std::string explanation = "OK";
    std::string buildHeaders(int contentLength, std::string url) const;
    void checkPermissions(const Request request);
    void print(std::string headers) const;
private:
    std::string defineContentType(std::string url) const;
};
