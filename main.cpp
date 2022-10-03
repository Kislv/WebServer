#include <fstream>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <iostream>

#include "include/http.h"
#include "include/file.h"
#include "include/threadPool.h"

#define PORT 8080
#define GET "GET"
#define HEAD "HEAD"
#define THREADQUANTITY 8

// #define DEBUG

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
    std::cout<<"SERVER IS STARTED"<<std::endl;
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);


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

    ThreadPool threadPool(THREADQUANTITY, DOCUMENT_ROOT);

    while (true) {
        if ((new_socket
            = accept(server_fd, (struct sockaddr*)&address,
                    (socklen_t*)&addrlen))
            < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        valread  =  read(new_socket, buffer, 1024);

        threadPool.AddToQueue(new_socket, std::string(buffer));
    }

	shutdown(server_fd, SHUT_RDWR);
	return 0;
}


