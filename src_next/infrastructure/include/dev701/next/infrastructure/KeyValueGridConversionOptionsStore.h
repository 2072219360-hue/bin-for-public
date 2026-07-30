#pragma once

#include <dev701/next/infrastructure/IGridConversionOptionsStore.h>

namespace dev701 {
namespace next {
namespace infrastructure {

class KeyValueGridConversionOptionsStore : public IGridConversionOptionsStore
{
public:
    bool load(
        const std::string& location,
        domain::GridConversionOptions* options,
        std::string* errorMessage);

    bool save(
        const std::string& location,
        const domain::GridConversionOptions& options,
        std::string* errorMessage);
};

} // namespace infrastructure
} // namespace next
} // namespace dev701
