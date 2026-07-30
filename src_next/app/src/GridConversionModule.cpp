#include <dev701/next/app/GridConversionModule.h>

namespace dev701 {
namespace next {
namespace app {

GridConversionModule::GridConversionModule(
    std::unique_ptr<infrastructure::IGridConversionOptionsStore> optionsStore)
    : optionsStore_(std::move(optionsStore))
    , initialized_(false)
{
}

ModuleDescriptor GridConversionModule::descriptor() const
{
    ModuleDescriptor value;
    value.id = "grid-conversion";
    value.displayName = "Grid Conversion";
    value.startupOrder = 100;
    return value;
}

bool GridConversionModule::initialize(std::string* errorMessage)
{
    if (!optionsStore_) {
        if (errorMessage != 0) {
            *errorMessage = "Grid conversion options store is not configured.";
        }
        return false;
    }
    initialized_ = true;
    return true;
}

void GridConversionModule::shutdown()
{
    initialized_ = false;
}

bool GridConversionModule::loadOptions(
    const std::string& location,
    domain::GridConversionOptions* options,
    std::string* errorMessage)
{
    if (!initialized_) {
        if (errorMessage != 0) {
            *errorMessage = "Grid conversion module is not initialized.";
        }
        return false;
    }
    return optionsStore_->load(location, options, errorMessage);
}

bool GridConversionModule::saveOptions(
    const std::string& location,
    const domain::GridConversionOptions& options,
    std::string* errorMessage)
{
    if (!initialized_) {
        if (errorMessage != 0) {
            *errorMessage = "Grid conversion module is not initialized.";
        }
        return false;
    }
    return optionsStore_->save(location, options, errorMessage);
}

} // namespace app
} // namespace next
} // namespace dev701
