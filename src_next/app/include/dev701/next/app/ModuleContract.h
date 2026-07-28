#pragma once

#include <string>

namespace dev701 {
namespace next {
namespace app {

struct ModuleDescriptor
{
    std::string id;
    std::string displayName;
    int startupOrder;
};

class IModule
{
public:
    virtual ~IModule() {}

    virtual ModuleDescriptor descriptor() const = 0;
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
};

class IModuleRegistry
{
public:
    virtual ~IModuleRegistry() {}

    virtual bool registerModule(IModule* module) = 0;
    virtual IModule* module(const std::string& id) const = 0;
    virtual bool startModules() = 0;
    virtual void stopModules() = 0;
};

} // namespace app
} // namespace next
} // namespace dev701
