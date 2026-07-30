#pragma once

#include <string>

#include <dev701/next/algorithms/IGridSimplificationService.h>

namespace dev701 {
namespace next {
namespace ui {

enum class GridSimplificationPageState
{
    Inactive,
    Ready,
    Running,
    Failed
};

class GridSimplificationPageController
{
public:
    explicit GridSimplificationPageController(
        algorithms::IGridSimplificationService& service);

    GridSimplificationPageState state() const;
    bool activate(std::string* errorMessage);
    void deactivate();
    bool simplify(
        const domain::ResultDocument& input,
        const algorithms::GridSimplificationRequest& request,
        domain::ResultDocument* output,
        std::string* errorMessage);

private:
    algorithms::IGridSimplificationService& service_;
    GridSimplificationPageState state_;
};

} // namespace ui
} // namespace next
} // namespace dev701
