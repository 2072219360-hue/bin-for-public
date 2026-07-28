#pragma once

#include <string>
#include <vector>

namespace dev701 {
namespace next {
namespace domain {

struct ResultFieldDescriptor
{
    std::string name;
    std::string unit;
    unsigned int componentCount;
};

struct ResultDocument
{
    std::string sourcePath;
    std::vector<ResultFieldDescriptor> fields;
};

} // namespace domain
} // namespace next
} // namespace dev701
