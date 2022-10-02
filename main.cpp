// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "include/http.h"
#include "include/file.h"

#define PORT 8080
#define GET "GET"
#define HEAD "HEAD"

#define DEBUG


int intLength (int n) { 
    int base = 10;
    int number_of_digits = 0;
    do {
        ++number_of_digits; 
        n /= base;
    } while (n);
    return number_of_digits;
}

int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);


	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true) {
        if ((new_socket
            = accept(server_fd, (struct sockaddr*)&address,
                    (socklen_t*)&addrlen))
            < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // TODO read image and others file extensions
        valread  =  read(new_socket, buffer, 1024);

        Request request;
        request.parse(buffer);

        #ifdef DEBUG
        // printf("%s\n", buffer);
        request.print();
        #endif

        Response response;

        // TODO check is file exist 404
        // std::cout<<"CHECKPOINT 1"<<std::endl;
        response.checkPermissions(request); 

        // std::cout<<"CHECKPOINT 2"<<std::endl;
        if (response.status == 200){
            fileExist(request, response);
        }

        std::string fullFile;
        int fileSize  = 0;
        if (response.status == 200 && (request.method == "GET" || request.method == "HEAD")) {
            // fullFile = readFile(request) ;
            fullFile = readFile(request);
            fileSize = fileLength(request);
        }


        std::string headers = response.buildHeaders(fileSize, request.url);


        #ifdef DEBUG
        // std::cout<<"FULLFILE::::::::::::::::::::::"<<fullFile<< std::endl;
        #endif
        response.print(headers);
        if (response.status == 200  && request.method == "GET") {
        // if (response.status == 200  && request.method == "GET" && request.content.size() < 200) {
            headers += fullFile;
            // headers += "\r\n\r\n";
        }
        if (request.method == "HEAD") {
            // headers += "\r\n\r\n";
            // headers += '\n';
        }


        std::cout<<"*****************************************************************************************************************"<<std::endl;
        if((send(new_socket, &(headers)[0], headers.size(), 0)) > 0){
            // printf("successed sending message\n");
        }     
        else{
            printf("failed sending message\n");
        }

        // printf("Hello message sent\n");

        // closing the connected socket
	    close(new_socket);
    }

	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}


