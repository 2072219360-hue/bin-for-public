#pragma once

#include <memory>

#include <dev701/next/app/ModuleContract.h>
#include <dev701/next/infrastructure/IGridConversionOptionsStore.h>

namespace dev701 {
namespace next {
namespace app {

class GridConversionModule : public IModule
{
public:
    explicit GridConversionModule(
        std::unique_ptr<infrastructure::IGridConversionOptionsStore> optionsStore);

    ModuleDescriptor descriptor() const;
    bool initialize(std::string* errorMessage);
    void shutdown();

    bool loadOptions(
        const std::string& location,
        domain::GridConversionOptions* options,
        std::string* errorMessage);
    bool saveOptions(
        const std::string& location,
        const domain::GridConversionOptions& options,
        std::string* errorMessage);

private:
    std::unique_ptr<infrastructure::IGridConversionOptionsStore> optionsStore_;
    bool initialized_;
};

} // namespace app
} // namespace next
} // namespace dev701
