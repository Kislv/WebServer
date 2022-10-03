#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../include/job.h"
#include "../include/http.h"
#include "../include/file.h"

void handle(unsigned int socket, const std::string &requestString){
    Request request;
    request.parse(requestString);

    #ifdef DEBUG
    // printf("%s\n", buffer);
    request.print();
    #endif

    Response response;

    response.checkPermissions(request); 

    if (response.status == 200){
        fileExist(request, response);
    }

    std::string fullFile;
    int fileSize  = 0;
    if (response.status == 200 && (request.method == "GET" || request.method == "HEAD")) {
        fullFile = readFile(request);
        fileSize = fileLength(request);
    }


    std::string headers = response.buildHeaders(fileSize, request.url);


    #ifdef DEBUG
    // std::cout<<"FULLFILE::::::::::::::::::::::"<<fullFile<< std::endl;
    response.print(headers);
    #endif
    if (response.status == 200  && request.method == "GET") {
        headers += fullFile;
    }

    #ifdef DEBUG
        std::cout<<"*****************************************************************************************************************"<<std::endl;
    #endif

    if((send(socket, &(headers)[0], headers.size(), 0)) <= 0){
        #ifdef DEBUG
            printf("failed sending message\n");
        #endif
    }     

    // closing the connected socket
    close(socket);
};
