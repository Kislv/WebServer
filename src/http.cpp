#include "../include/http.h"
#include <stdio.h>
#include <time.h>
#include <unordered_map>
#include <filesystem>
#include <algorithm>


const std::string HTMLEXTENSION = "html";
const std::string DEFAULTFILE = "index.html";
const std::unordered_map<std::string, std::string> extensionsContentType {
    {"html", "text/html"},
    {"css", "text/css"},
    {"js", "application/javascript"},
    {"jpg", "image/jpeg"},
    {"jpeg", "image/jpeg"},
    {"png", "image/png"},
    {"gif", "image/gif"},
    {"swf", "application/x-shockwave-flash"}
};

void Request::parse(std::string request){
    int methodEnd = request.find(' ');
    method = request.substr(0, methodEnd);
    request = request.substr(methodEnd+1, request.length());
    int urlEnd = request.find(' ');
    url = request.substr(0, urlEnd);

    auto startQueryString = url.find("?");
    if (startQueryString != std::string::npos){
        url = url.substr(0, startQueryString);
    }
    percentDecode();

}

void Request::print(){
    std::cout<<"--------------------------REQUEST--------------------------"<<std::endl;
    std::cout<<"Method = "<< method <<std::endl;
    std::cout<<"Url = "<< url <<std::endl;
    std::cout<<std::endl;
    std::cout<<"-----------------------------------------------------------"<<std::endl;

}

std::string Request::buildFilePath() {
    // Without "/"
    url = url.substr(1);
    size_t lastPoint = url.find_last_of('.');
    size_t lastSlash = url.find_last_of('/');

    if (lastPoint != std::string::npos && lastSlash != std::string::npos && lastSlash > lastPoint && extensionsContentType.find(url.substr(lastPoint+1, lastSlash - lastPoint - 1)) != extensionsContentType.end()){
        return url;
    }
    if(url == "" || url[url.length() - 1] =='/' && url.find_last_of('.') ) {
        isIndex = true;
        return url + DEFAULTFILE;
    }
    return url;
};

std::string Response::defineContentType(std::string url) const {

    std::size_t pos = url.find_last_of('.');
    if (pos == std::string::npos) {
        url += DEFAULTFILE;
        pos = url.find_last_of('.');
    }
    auto contentTypeIt = extensionsContentType.find(url.substr(pos + 1));
    if (contentTypeIt != extensionsContentType.end()){
        return contentTypeIt->second;
    } else {
        #ifdef DEBUG
            perror("ERROR: undefined file extension");
        #endif 
        return "none";
    }
}

std::string Response::buildHeaders(int contentLength, std::string url) const {
    std::string headers = "";
    headers  +=  protocol + " ";
    headers  +=  std::to_string(status) + " ";
    headers  +=  explanation + "\r\n";

    // Generate current date
    headers += "Date: ";
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    headers += buf;
    headers += "\r\n";

    headers += "Server: KiselevServer/1\r\n";
   
    if (status == 200) {
        headers  +=  "Content-length: " + std::to_string(contentLength);
        headers  +=  "\r\n";
        headers  +=  "Content-Type: ";
        headers  +=  this->defineContentType(url);
        headers += "\r\n";
        //TODO sprintf, cause concatination is long
    }
    headers += "Connection: close\r\n\r\n";

    return headers;
};

void Response::checkPermissions(const Request request) {
    if (request.method != "GET" && request.method != "HEAD") {
        status = 405;
        explanation = "Method Not Allowed";
    };
    
    if (request.url.find("../") != std::string::npos || request.url.find("~") != std::string::npos || request.url.find("//") != std::string::npos){
        status = 403;
        explanation = "Forbidden";
    }
}

void Response::print(std::string headers) const {
    std::cout<<"--------------------------Response--------------------------"<<std::endl;
    std::cout<<headers;
    std::cout<<"------------------------------------------------------------"<<std::endl;
};


void Request::percentDecode(){
    std::string newURL = "";
    for (std::string::iterator i = url.begin(); i != url.end(); i++){
        if (*i == '%') {
            newURL += 16*(toHex(i[1])) + (toHex(i[2]));
            i += 2;
        } else {
            newURL += *i;
        }
    }
    url = newURL;
};

char Request::toHex(char c){
    if (!isxdigit(c)){
        perror("ERROR: char is not 16-bit digit");
        exit(2);
    }
    if (c >= '0' && c <= '9'){
        return c - '0';
    } else {
        if (c >= 'a' && c <= 'f') {
            return 10 + (c - 'a');
        } else {
        return 10 + (c - 'A');
        }
    }
    
}

