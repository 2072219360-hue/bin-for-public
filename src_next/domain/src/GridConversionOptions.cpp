#include <dev701/next/domain/GridConversionOptions.h>

namespace dev701 {
namespace next {
namespace domain {

GridConversionOptions::GridConversionOptions()
    : xSize(100)
    , ySize(100)
    , zSize(100)
    , interval(1.0)
{
}

bool GridConversionOptions::isValid() const
{
    return !sourceFilePath.empty()
        && xSize > 0
        && ySize > 0
        && zSize > 0
        && interval > 0.0;
}

} // namespace domain
} // namespace next
} // namespace dev701
