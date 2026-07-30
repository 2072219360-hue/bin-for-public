#pragma once

#include <memory>
#include <vector>

#include <dev701/next/app/ModuleContract.h>

namespace dev701 {
namespace next {
namespace app {

class ModuleRegistry : public IModuleRegistry
{
public:
    ModuleRegistry();
    ~ModuleRegistry();

    bool registerModule(
        std::unique_ptr<IModule> module,
        std::string* errorMessage);
    IModule* module(const std::string& id) const;
    bool startModules(std::string* errorMessage);
    void stopModules();

private:
    struct Entry
    {
        Entry(std::unique_ptr<IModule> value, ModuleDescriptor valueDescriptor);

        std::unique_ptr<IModule> instance;
        ModuleDescriptor descriptor;
        bool started;
    };

    std::vector<Entry> modules_;
    bool startAttempted_;
};

} // namespace app
} // namespace next
} // namespace dev701
