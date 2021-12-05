#include "DltInjection.hpp"
#include <iostream>
#include <thread>

DltInjection::DltInjection(std::string servIp, std::size_t port)
: m_servIp(servIp)
, m_port{port}
{
    dlt_client_init(&m_dltClient, 1);
    m_dltClient.mode = DLT_CLIENT_MODE_TCP;

    if (dlt_client_set_server_ip(&m_dltClient, strBegin(m_servIp)) == -1) {
        std::cout << "could not set dlt server ip" << m_servIp << "\n";
    }

    if (dlt_client_connect(&m_dltClient, 1) == DLT_RETURN_ERROR) {
        std::cout << "could not connect dlt client\n";
    }
}

DltInjection::~DltInjection()
{
    dlt_client_cleanup(&m_dltClient, 1);
}

void DltInjection::send(InjectionMessage msg)
{
    using namespace std::literals::chrono_literals;
    // printf("Send injection message:\n");
    // printf("AppId: %s\n", m_appId.data());
    // //printf("ConId: %s\n", m_ctxId.data());
    // printf("ServiceId: %d\n", serviceId);
    // printf("Message: %s\n", msg.data());
    // /* send control message in ascii */
    if (dlt_client_send_inject_msg(&m_dltClient, msg.appId.begin(), strBegin(msg.ctxId),
                                   msg.serviceId, msg.rawMessage.data(), msg.rawMessage.size()) != DLT_RETURN_OK)
    {
        std::cout << "Injection could not be sent\n";
    }

    if(msg.delay > 0ms)
    {
        //simple sleep mechanism, could be improved in the future
        std::this_thread::sleep_for(msg.delay);
    }
}

void DltInjection::send(std::vector<InjectionMessage> msgBundle)
{
    for (const auto& msg : msgBundle)
    {
        send(msg);
    }
}