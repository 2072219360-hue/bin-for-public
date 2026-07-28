#pragma once

#include <string>

namespace dev701 {
namespace next {
namespace ui {

struct PageDescriptor
{
    std::string id;
    std::string title;
    std::string ribbonCategory;
    int ribbonOrder;
};

// Qt widgets implement this contract through a thin adapter in ui/.
class IPage
{
public:
    virtual ~IPage() {}

    virtual PageDescriptor descriptor() const = 0;
    virtual bool activate(std::string* errorMessage) = 0;
    virtual void deactivate() = 0;
};

} // namespace ui
} // namespace next
} // namespace dev701
