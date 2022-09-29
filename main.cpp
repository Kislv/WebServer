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

#define PORT 8080
#define GET "GET"
#define HEAD "HEAD"

const std::string DOCUMENT_ROOT = "../Storage/";

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
	// char* hello = "Hello from server";
	// char* hello = "HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";

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
	if ((new_socket
		= accept(server_fd, (struct sockaddr*)&address,
				(socklen_t*)&addrlen))
		< 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	valread  =  read(new_socket, buffer, 1024);
	printf("%s\n", buffer);

    Request request;
    request.parse(buffer);
    request.print();

    std::ifstream myfile; 
    myfile.open(DOCUMENT_ROOT + request.buildFilePath());
    // myfile.open("../page/_html.html");
    std::string mystring, fullFile;


    if ( myfile.is_open() ) { // always check whether the file is open
        while (myfile.good()) {
            std::getline(myfile, mystring);
            // myfile >> mystring;
            fullFile += mystring;
        // std::cout << mystring;
        // write(new_socket,  &(mystring[0]), mystring.size()+1);
        }
        // std::cout <<"My string: "<< mystring << std::endl; // pipe stream's content to standard output
    }
    else{
    std::cout <<"ERROR OPEN My string: "<< mystring << std::endl; // pipe stream's content to standard output

    }

    Response response;

    response.build(request);
    std::string headers = response.buildHeaders(fullFile.size(), request.url);
    // std::string headers = "";
    // headers  +=  response.protocol "HTTP/1.1 200 OK\n";
    // headers  +=  "HTTP/1.1 200 OK\n";
    // headers  +=  "Content-length: " + fullFile.size();
    // headers  +=  "\n";
    // headers  +=  "Content-Type: text/html\n\n";

    // write(new_socket, "HTTP/1.1 200 OK\n", 16);
    // mystring = "Content-length: " + fullFile.size();
    // mystring += "\n";
    // write(new_socket, &(mystring[0]), 17 + intLength(fullFile.size()));
    // write(new_socket, "Content-Type: text/html\n\n", 25);
    // write(new_socket, "<html><body><H1>Hello world</H1></body></html>",46); 
    // if((send(new_socket, &(mystring[0]), mystring.size()+1, 0)) > 0){
    headers += fullFile;
    std::cout<< headers<<std::endl;
    if((send(new_socket, &(headers)[0], headers.size()+1, 0)) > 0){
        printf("successed sending message\n");
    }     
    else{
        printf("failed sending message\n");
    }
	// printf("Hello message sent\n");

	// closing the connected socket

	close(new_socket);
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}

