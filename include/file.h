#pragma once

#include <filesystem>
#include <fstream>

#include "http.h"

const std::string DOCUMENT_ROOT = "../../Other/http-test-suite/";
// const std::string DOCUMENT_ROOT = "../../Other/http-test-suite/httptest/";
// const std::string DOCUMENT_ROOT = "../Storage/";

std::string readFile (const Request request);
void fileExist (Request request,  Response &response);
std::uintmax_t  fileLength (Request request);
