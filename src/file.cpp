#include "../include/file.h"

std::string readFile (Request request) {

    std::ifstream file(DOCUMENT_ROOT + request.buildFilePath(), std::ios::in | std::ios::binary);
    std::stringstream fl;
    if (file.is_open()) {
        fl << file.rdbuf();
    }

    file.close();
    return fl.str();

    // std::ifstream myfile; 
    // myfile.open(DOCUMENT_ROOT + request.buildFilePath());
    // // myfile.open("../page/_html.html");
    // std::string mystring, fullFile;


    // if ( myfile.is_open() ) { // always check whether the file is open
    //     while (myfile.good()) {
    //         std::getline(myfile, mystring);
    //         // myfile >> mystring;
    //         fullFile += mystring;
    //     // std::cout << mystring;
    //     // write(new_socket,  &(mystring[0]), mystring.size()+1);
    //     }
    //     // std::cout <<"My string: "<< mystring << std::endl; // pipe stream's content to standard output
    // }
    // else{
    // std::cout <<"ERROR OPEN My string: "<< mystring << std::endl; // pipe stream's content to standard output

    // }
    // return fullFile;
}

void fileExist (Request request,  Response &response){
    if (!std::filesystem::exists(DOCUMENT_ROOT + request.buildFilePath())) {
        if (!request.isIndex){
            response.status = 404;
            response.explanation = "Not Found";
            return;
        } else {
            response.status = 403;
            std::cout<<"403 45"<<std::endl;
            response.explanation = "Forbidden";
            return;
        }
    }
};

std::uintmax_t  fileLength (Request request){
    return std::filesystem::file_size(DOCUMENT_ROOT + request.buildFilePath());
};

