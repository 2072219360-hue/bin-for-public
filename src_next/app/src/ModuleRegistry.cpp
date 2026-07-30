#include <dev701/next/app/ModuleRegistry.h>

#include <algorithm>

namespace dev701 {
namespace next {
namespace app {

namespace {

void setError(std::string* errorMessage, const std::string& value)
{
    if (errorMessage != 0) {
        *errorMessage = value;
    }
}

} // namespace

ModuleRegistry::Entry::Entry(
    std::unique_ptr<IModule> value,
    ModuleDescriptor valueDescriptor)
    : instance(std::move(value))
    , descriptor(valueDescriptor)
    , started(false)
{
}

ModuleRegistry::ModuleRegistry()
    : startAttempted_(false)
{
}

ModuleRegistry::~ModuleRegistry()
{
    stopModules();
}

bool ModuleRegistry::registerModule(
    std::unique_ptr<IModule> moduleValue,
    std::string* errorMessage)
{
    if (startAttempted_) {
        setError(errorMessage, "Modules cannot be registered after startup begins.");
        return false;
    }
    if (!moduleValue) {
        setError(errorMessage, "A null module cannot be registered.");
        return false;
    }

    const ModuleDescriptor descriptor = moduleValue->descriptor();
    if (descriptor.id.empty()) {
        setError(errorMessage, "A module id is required.");
        return false;
    }
    if (module(descriptor.id) != 0) {
        setError(errorMessage, "A module with the same id is already registered: " + descriptor.id);
        return false;
    }

    modules_.push_back(Entry(std::move(moduleValue), descriptor));
    return true;
}

IModule* ModuleRegistry::module(const std::string& id) const
{
    for (std::vector<Entry>::const_iterator it = modules_.begin(); it != modules_.end(); ++it) {
        if (it->descriptor.id == id) {
            return it->instance.get();
        }
    }
    return 0;
}

bool ModuleRegistry::startModules(std::string* errorMessage)
{
    if (startAttempted_) {
        setError(errorMessage, "Module startup has already been attempted.");
        return false;
    }

    startAttempted_ = true;
    std::sort(
        modules_.begin(),
        modules_.end(),
        [](const Entry& left, const Entry& right) {
            if (left.descriptor.startupOrder != right.descriptor.startupOrder) {
                return left.descriptor.startupOrder < right.descriptor.startupOrder;
            }
            return left.descriptor.id < right.descriptor.id;
        });
    for (std::vector<Entry>::iterator it = modules_.begin(); it != modules_.end(); ++it) {
        std::string moduleError;
        if (!it->instance->initialize(&moduleError)) {
            setError(
                errorMessage,
                "Module initialization failed for " + it->descriptor.id + ": " + moduleError);
            stopModules();
            return false;
        }
        it->started = true;
    }
    return true;
}

void ModuleRegistry::stopModules()
{
    for (std::vector<Entry>::reverse_iterator it = modules_.rbegin(); it != modules_.rend(); ++it) {
        if (it->started) {
            it->instance->shutdown();
            it->started = false;
        }
    }
}

} // namespace app
} // namespace next
} // namespace dev701
