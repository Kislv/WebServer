#include "../include/file.h"

std::string readFile (Request request) {

    std::ifstream file(DOCUMENT_ROOT + request.buildFilePath(), std::ios::in | std::ios::binary);
    std::stringstream fl;
    if (file.is_open()) {
        fl << file.rdbuf();
    }

    file.close();
    return fl.str();
}

void fileExist (Request request,  Response &response){
    std::string filePath = DOCUMENT_ROOT + request.buildFilePath();

    if (!std::filesystem::exists(filePath)) {
        if (!request.isIndex){
            response.status = 404;
            response.explanation = "Not Found";
            return;
        } else {
            response.status = 403;
            response.explanation = "Forbidden";
            return;
        }
    }
};

std::uintmax_t  fileLength (Request request){
    return std::filesystem::file_size(DOCUMENT_ROOT + request.buildFilePath());
};

