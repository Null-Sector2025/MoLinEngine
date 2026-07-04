#pragma once
#include <string>
#include <functional>

namespace MoLin::Network {

class HttpClient {
public:
    static std::string Get(const std::string& url);
    static std::string Post(const std::string& url, const std::string& body);
};

} // namespace MoLin::Network
