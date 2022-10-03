#pragma once

#include <filesystem>
#include <fstream>

#include "http.h"
// For docker container
const std::string DOCUMENT_ROOT = "../";
// const std::string DOCUMENT_ROOT = "/home/viktor/Projects/Technopark/3Semestr/Highload/Other/http-test-suite/";
// For local running
// const std::string DOCUMENT_ROOT = "../../Other/http-test-suite/";
// const std::string DOCUMENT_ROOT = "../../Other/http-test-suite/httptest/";
// const std::string DOCUMENT_ROOT = "../Storage/";

std::string readFile (const Request request);
void fileExist (Request request,  Response &response);
std::uintmax_t  fileLength (Request request);
