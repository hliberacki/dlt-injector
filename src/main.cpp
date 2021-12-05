#include <string>
#include <iostream>
#include <chrono>
#include <memory>

#include <dlt/dlt_client.h>
#include <dlt/dlt_user.h>

#include "DltInjection.hpp"
#include "InjectionVariants.hpp"

using namespace std::literals::chrono_literals;

int main()
{
    std::string appId = "EXA1";
    std::string ctxId = "CON";
    DltInjection injector;
    std::vector<InjectionMessage> msgBundle;
    std::vector<InjectionMessage> msgNoDelayBundle;
    for(auto i = 0; i < 5; ++i) {
        InjectionMessage msg(appId, ctxId, 0x1000, "foo", 1000ms);
        msgBundle.push_back(msg);
        InjectionMessage msgNoDelay(appId, ctxId, 0x1000, "foo");
        msgNoDelayBundle.push_back(msgNoDelay);
    }
    std::vector<std::unique_ptr<InjectionVariant>> testCase;
    testCase.push_back(std::make_unique<BundleVariant>(msgBundle));
    testCase.push_back(std::make_unique<BundleVariant>(msgNoDelayBundle));
    testCase.push_back(std::make_unique<RepeaterVariant>(Repeater(msgNoDelayBundle, 1000ms)));

    for (auto& node: testCase)
    {
        do
        {
            injector.send(node->next());
        } while (node->doContinue());
    }
    return 0;
}