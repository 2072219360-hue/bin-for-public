#pragma once

#include <string>

#include <dev701/next/domain/ResultDocument.h>

namespace dev701 {
namespace next {
namespace algorithms {

struct GridSimplificationRequest
{
    double targetReduction;
    bool preserveBoundary;

    GridSimplificationRequest()
        : targetReduction(0.0)
        , preserveBoundary(true)
    {
    }
};

class IGridSimplificationService
{
public:
    virtual ~IGridSimplificationService() {}

    virtual bool simplify(
        const domain::ResultDocument& input,
        const GridSimplificationRequest& request,
        domain::ResultDocument* output,
        std::string* errorMessage) = 0;
};

} // namespace algorithms
} // namespace next
} // namespace dev701
