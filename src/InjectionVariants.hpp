#ifndef DLT_INJECTIONS_SRC_INJECTION_VARIANTS
#define DLT_INJECTIONS_SRC_INJECTION_VARIANTS

#include "InjectionMessage.hpp"

//Base class for all types of messages/bundles wrappers
struct InjectionVariant
{

    // check if we still need to send data
    // needed for repeater
    virtual bool doContinue() = 0;

    //get next chunk of data
    virtual std::vector<InjectionMessage> next() = 0;

    //by default calls are synchroniouse
    virtual bool isAsync() {return false; }
};



class BundleVariant : public InjectionVariant
{
public:
    BundleVariant(std::vector<InjectionMessage> msgBundle)
    : messages {msgBundle}
    {}

    //Bundle is a single run, hence it's done once
    virtual bool doContinue() override { return false; }

    virtual std::vector<InjectionMessage> next() override
    {
        return messages;
    }

    std::vector<InjectionMessage> messages;
};

class MessageVariant : public BundleVariant
{
    MessageVariant(InjectionMessage msg)
    : BundleVariant {{msg}}
    {}
};

class RepeaterVariant : public InjectionVariant
{
public:
    RepeaterVariant(Repeater repeater)
    : messageRepeater(repeater)
    , currentIdx{0}
    {
    }

    virtual bool doContinue() override
    {
        using namespace std::chrono;
        if (messageRepeater.type == Repeater::Type::TIME)
        {
            return duration_cast<milliseconds>(start - steady_clock::now()) < messageRepeater.duration;
        }
        else
        {
            return currentIdx <= messageRepeater.repeatForN;
        }
    }

    virtual std::vector<InjectionMessage> next() override
    {
       using namespace std::chrono;
       if (messageRepeater.type == Repeater::Type::COUNTER)
       {
           ++currentIdx;
       }
       // Since messages in the queue are possibly not ad the bottom of the list
       // and by default all messages are executed synchronualisly
       // first fetch of data, starts the timer - since this is indicates that message is beeing processed.
       else if (duration_cast<milliseconds>(start.time_since_epoch()).count() == 0)
       {
           start = steady_clock::now();
       }
       return messageRepeater.msgBundle;
    }
private:
    Repeater messageRepeater;
    std::size_t currentIdx;
    std::chrono::time_point<std::chrono::steady_clock> start;
};
#endif