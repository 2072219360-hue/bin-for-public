#pragma once

#include <string>

#include <dev701/next/domain/GridConversionOptions.h>

namespace dev701 {
namespace next {
namespace infrastructure {

class IGridConversionOptionsStore
{
public:
    virtual ~IGridConversionOptionsStore() {}

    virtual bool load(
        const std::string& location,
        domain::GridConversionOptions* options,
        std::string* errorMessage) = 0;

    virtual bool save(
        const std::string& location,
        const domain::GridConversionOptions& options,
        std::string* errorMessage) = 0;
};

} // namespace infrastructure
} // namespace next
} // namespace dev701
