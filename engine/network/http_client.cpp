#include "http_client.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>

namespace MoLin::Network {

std::string HttpClient::Get(const std::string& url) {
    std::string command = "curl -s \"" + url + "\"";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) return "";
    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }
    return result;
}

std::string HttpClient::Post(const std::string& url, const std::string& body) {
    std::string command = "curl -s -X POST -d \"" + body + "\" \"" + url + "\"";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) return "";
    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }
    return result;
}

} // namespace MoLin::Network
