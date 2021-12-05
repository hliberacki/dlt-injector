#ifndef DLT_INJECTIONS_SRC_INJECTION_MESSAGE
#define DLT_INJECTIONS_SRC_INJECTION_MESSAGE

#include <array>
#include <vector>
#include <chrono>

struct InjectionMessage
{
    InjectionMessage(std::string _appId, std::string _ctxId, const std::uint32_t _serviceId, std::string msg)
    : serviceId{_serviceId}
    , ctxId(_ctxId)
    , rawMessage(msg.begin(), msg.end())
    , delay{0}
    {
        std::copy(_appId.begin(), _appId.end(), appId.data());
    }

    InjectionMessage(std::string _appId, std::string _ctxId, const std::uint32_t _serviceId, std::string msg, std::chrono::milliseconds _delay)
    : InjectionMessage(_appId, _ctxId, _serviceId, msg)
    {
        delay = _delay;
    }

    const std::uint32_t serviceId;
    std::string ctxId;
    std::vector<std::uint8_t> rawMessage;
    std::array<char, DLT_ID_SIZE> appId;
    std::chrono::milliseconds delay;
};

struct Repeater
{
    enum Type {
        TIME,
        COUNTER
    };
    explicit Repeater(std::vector<InjectionMessage> bundle, std::chrono::milliseconds doFor)
    : msgBundle{bundle}
    , duration{doFor}
    , type {Type::TIME }
    {}

    explicit Repeater(std::vector<InjectionMessage> bundle, std::size_t doFor)
    : msgBundle{bundle}
    , duration{doFor}
    , type {Type::COUNTER }
    {}

    std::chrono::milliseconds duration;
    std::size_t repeatForN;
    std::vector<InjectionMessage> msgBundle;
    const Repeater::Type type;
};

#endif // DLT_INJECTIONS_SRC_INJECTION_MESSAGE