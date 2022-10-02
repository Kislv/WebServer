#include "../include/http.h"
#include <stdio.h>
#include <time.h>
#include <unordered_map>
#include <filesystem>
#include <algorithm>


const std::string HTMLEXTENSION = "html";
const std::string DEFAULTFILE = "index.html";


void Request::parse(std::string request){
    int methodEnd = request.find(' ');
    method = request.substr(0, methodEnd);
    request = request.substr(methodEnd+1, request.length());
    int urlEnd = request.find(' ');
    url = request.substr(0, urlEnd);
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
    if(url == "") {
        return "index.html";
    }

    if (url[url.length() - 1] == '/') {
        return url + DEFAULTFILE;
    }
    return url;
};

std::string Response::defineContentType(std::string url) const {
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
    // TODO get pos of last point
    std::size_t pos = url.find('.');
    if (pos == std::string::npos) {
        url += DEFAULTFILE;
        pos = url.find('.');
    }
    auto contentTypeIt = extensionsContentType.find(url.substr(pos + 1));
    if (contentTypeIt != extensionsContentType.end()){
        return contentTypeIt->second;
    } else {
        perror("ERROR: undefined file extension");
        return "none";
        // exit(1);
    }
}

std::string Response::buildHeaders(int contentLength, std::string url) const {
    std::string headers = "";
    headers  +=  protocol + " ";
    headers  +=  std::to_string(status) + " ";
    headers  +=  explanation + "\n";

    // Generate current date
    headers += "Date: ";
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    headers += buf;
    headers += "\n";

    headers += "Server: KiselevServer/1\n";
   
    if (status == 200) {
        headers  +=  "Content-length: " + std::to_string(contentLength);
        headers  +=  "\n";
        headers  +=  "Content-Type: ";
        headers  +=  this->defineContentType(url);
        headers += "\n";

        //TODO sprintf, cause concatination is long
    }
    
    headers += "Connection: close\n\n";

    // print(headers);
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
    std::cout<<headers<<std::endl;
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
        return 10 + (c - 'A');
    }
    
}

