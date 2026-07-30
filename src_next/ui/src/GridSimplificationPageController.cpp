#include <dev701/next/ui/GridSimplificationPageController.h>

namespace dev701 {
namespace next {
namespace ui {

GridSimplificationPageController::GridSimplificationPageController(
    algorithms::IGridSimplificationService& service)
    : service_(service)
    , state_(GridSimplificationPageState::Inactive)
{
}

GridSimplificationPageState GridSimplificationPageController::state() const
{
    return state_;
}

bool GridSimplificationPageController::activate(std::string* errorMessage)
{
    if (state_ == GridSimplificationPageState::Running) {
        if (errorMessage != 0) {
            *errorMessage = "The grid simplification page is already running.";
        }
        return false;
    }
    state_ = GridSimplificationPageState::Ready;
    return true;
}

void GridSimplificationPageController::deactivate()
{
    if (state_ != GridSimplificationPageState::Running) {
        state_ = GridSimplificationPageState::Inactive;
    }
}

bool GridSimplificationPageController::simplify(
    const domain::ResultDocument& input,
    const algorithms::GridSimplificationRequest& request,
    domain::ResultDocument* output,
    std::string* errorMessage)
{
    if (state_ != GridSimplificationPageState::Ready) {
        if (errorMessage != 0) {
            *errorMessage = "Activate the grid simplification page before running an operation.";
        }
        return false;
    }
    if (output == 0) {
        if (errorMessage != 0) {
            *errorMessage = "A result destination is required.";
        }
        return false;
    }
    if (request.targetReduction < 0.0 || request.targetReduction >= 1.0) {
        if (errorMessage != 0) {
            *errorMessage = "Target reduction must be greater than or equal to 0 and less than 1.";
        }
        return false;
    }

    state_ = GridSimplificationPageState::Running;
    if (!service_.simplify(input, request, output, errorMessage)) {
        state_ = GridSimplificationPageState::Failed;
        return false;
    }
    state_ = GridSimplificationPageState::Ready;
    return true;
}

} // namespace ui
} // namespace next
} // namespace dev701
