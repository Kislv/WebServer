#include "../include/http.h"
#include <stdio.h>
#include <time.h>
#include <map>

const std::string HTMLEXTENSION = "html";
const std::string DEFAULTFILE = "index.html";


// const std::string SLASH = "/";

void Request::parse(std::string request){
    int methodEnd = request.find(' ');
    method = request.substr(0, methodEnd);
    request = request.substr(methodEnd+1, request.length());
    int urlEnd = request.find(' ');
    url = request.substr(0, urlEnd);
    

    // method = "";
    // std::cout<<"request.size() = "<< request.size()<<std::endl;
    // for (int i = 0; i < request.size(); i++){
    //     if (request[i] == *" "){
    //         return;
    //     } else {
    //         method += request[i];
    //     }
    // }
}

// void methodToEnum(std::string str){
//     if (str == "GET"){
//         method = Method(GET);
//     }
// }

void Request::print(){
    std::cout<<"REQUEST"<<std::endl;
    std::cout<<"Method = "<< method <<std::endl;
    std::cout<<"Url = "<< url <<std::endl;
    // std::cout<<"Protocol = "<< protocol <<std::endl;
    // for (int i = 0; i < headers.size(); i++){
    //     std::cout<< "Name = " << headers[i].first<<" Value = "<< headers[i].second<<std::endl;
    // }
    // std::cout<<"Content = "<< std::string(content.begin(), content.end()) <<std::endl;
    std::cout<<std::endl;
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

    // TODO check security with specific symbols
    // if (url.substr(url.length() - HTMLEXTENSION.length(), url.length()) == HTMLEXTENSION) {
    //     return  url;
    // }
    return url;
};

std::string Response::defineContentType(std::string url) const {
    const std::map<std::string, std::string> extensionsContentType {
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
    if (pos != std::string::npos) {
        std::cout<<"PPPPPPPPPPPPPPPPPPPPPPPOOOOOOOOOOOOOSSSSSSSSSSS "<< pos<<std::endl;
        std::cout<<"SUUUUBSTRSUUUUBSTRSUUUUBSTRSUUUUBSTRSUUUUBSTRSUUUUBSTRSUUUUBSTR "<< url.substr(pos + 1)<<std::endl;
        // return url.substr(pos + 1); 
        auto contentTypeIt = extensionsContentType.find(url.substr(pos + 1));
        if (contentTypeIt != extensionsContentType...()){
            return contentTypeIt->second;
        } else {
            // error(1);
            exit(1);
        }
    } else {
        return HTMLEXTENSION;
    }
    // for ( std::string::iterator it=url.end(); it!=url.begin(); --it){
    //     std::cout << *it;
    //     if (*it == '.') {
    //         return url.substr(url.find('.'));
    //     }
    //     std::cout << '\n';
    // }
}

std::string Response::buildHeaders(int contentLength, std::string url) const {
    std::string headers = "";
    headers  +=  protocol + " ";
    headers  +=  std::to_string(status) + " ";
    headers  +=  explanation + "\n";
    headers += "Server: KiselevServer/1\n";

    // Generate current date
    headers += "Date: ";
    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
    headers += buf;
    headers += "\n";
    
    headers += "Connection: closed\n";

    if (status == 200) {
        headers  +=  "Content-length: " + std::to_string(contentLength);
        headers  +=  "\n";
        
        // headers  +=  "Content-Type: text/html\n\n";
        headers  +=  "Content-Type: ";
        headers  +=  this->defineContentType(url);
        headers += "\n\n";

        headers  +=  "\n\n";
        // headers  +=  "Content-Type: text/html\n\n";
        //TODO sprintf, cause concatination is long
    }
    return headers;
};

std::string Response::checkPermissions(Request request) {
    if (request.method != "GET" && request.method != "HEAD") {
        status = 405;
        explanation = "Method Not Allowed";
    };

    if (request.url.find("../") != request.url.end() || request.url.find("~") != request.url.end() || request.url.find("//") != request.url.end()){
        status = 403;
        explanation = "Forbidden";
    
    }
}
