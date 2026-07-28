#pragma once

#include <string>

namespace dev701 {
namespace next {
namespace domain {

// Future replacement for the legacy platforms::echv::argData value object.
class GridConversionOptions
{
public:
    GridConversionOptions();

    bool isValid() const;

    std::string sourceFilePath;
    unsigned int xSize;
    unsigned int ySize;
    unsigned int zSize;
    double interval;
};

} // namespace domain
} // namespace next
} // namespace dev701
