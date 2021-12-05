#ifndef DLT_INJECTIONS_SRC_DLT_INJECTION
#define DLT_INJECTIONS_SRC_DLT_INJECTION

#include <dlt/dlt_client.h>
#include <array>
#include <string>
#include <chrono>
#include <vector>
#include "InjectionMessage.hpp"

namespace {
char* strBegin(std::string& str)
{
    return &str[0];
}
}

class DltInjection
{
    public:
    DltInjection(std::string servIp = "0.0.0.0", std::size_t port = 3490);

    void send(InjectionMessage msg);
    void send(std::vector<InjectionMessage> msgBundle);
    //void send()

    ~DltInjection();
    private:
    DltClient m_dltClient;
    std::string m_servIp;
    uint64_t m_port;
};

#endif // DLT_INJECTIONS_SRC_DLT_INJECTION