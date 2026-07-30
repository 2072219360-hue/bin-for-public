#include <dev701/next/infrastructure/KeyValueGridConversionOptionsStore.h>

#include <fstream>
#include <sstream>

namespace dev701 {
namespace next {
namespace infrastructure {

namespace {

void setError(std::string* errorMessage, const std::string& value)
{
    if (errorMessage != 0) {
        *errorMessage = value;
    }
}

bool parseUnsigned(const std::string& value, unsigned int* result)
{
    std::istringstream input(value);
    unsigned long parsed = 0;
    char trailing = 0;
    if (!(input >> parsed) || (input >> trailing) || parsed == 0) {
        return false;
    }
    *result = static_cast<unsigned int>(parsed);
    return true;
}

bool parseDouble(const std::string& value, double* result)
{
    std::istringstream input(value);
    char trailing = 0;
    if (!(input >> *result) || (input >> trailing) || *result <= 0.0) {
        return false;
    }
    return true;
}

} // namespace

bool KeyValueGridConversionOptionsStore::load(
    const std::string& location,
    domain::GridConversionOptions* options,
    std::string* errorMessage)
{
    if (options == 0) {
        setError(errorMessage, "A destination options object is required.");
        return false;
    }

    std::ifstream input(location.c_str());
    if (!input) {
        setError(errorMessage, "Unable to open grid conversion options: " + location);
        return false;
    }

    domain::GridConversionOptions loaded;
    std::string line;
    unsigned int lineNumber = 0;
    while (std::getline(input, line)) {
        ++lineNumber;
        if (line.empty() || line[0] == '#') {
            continue;
        }

        const std::string::size_type separator = line.find('=');
        if (separator == std::string::npos) {
            setError(errorMessage, "Invalid options line " + std::to_string(lineNumber));
            return false;
        }

        const std::string key = line.substr(0, separator);
        const std::string value = line.substr(separator + 1);
        bool valid = true;
        if (key == "sourceFilePath") {
            loaded.sourceFilePath = value;
        } else if (key == "xSize") {
            valid = parseUnsigned(value, &loaded.xSize);
        } else if (key == "ySize") {
            valid = parseUnsigned(value, &loaded.ySize);
        } else if (key == "zSize") {
            valid = parseUnsigned(value, &loaded.zSize);
        } else if (key == "interval") {
            valid = parseDouble(value, &loaded.interval);
        }

        if (!valid) {
            setError(errorMessage, "Invalid value on options line " + std::to_string(lineNumber));
            return false;
        }
    }

    if (!loaded.isValid()) {
        setError(errorMessage, "The options file is incomplete or invalid: " + location);
        return false;
    }

    *options = loaded;
    return true;
}

bool KeyValueGridConversionOptionsStore::save(
    const std::string& location,
    const domain::GridConversionOptions& options,
    std::string* errorMessage)
{
    if (!options.isValid()) {
        setError(errorMessage, "Invalid grid conversion options cannot be saved.");
        return false;
    }

    std::ofstream output(location.c_str(), std::ios::out | std::ios::trunc);
    if (!output) {
        setError(errorMessage, "Unable to write grid conversion options: " + location);
        return false;
    }

    output << "sourceFilePath=" << options.sourceFilePath << "\n";
    output << "xSize=" << options.xSize << "\n";
    output << "ySize=" << options.ySize << "\n";
    output << "zSize=" << options.zSize << "\n";
    output << "interval=" << options.interval << "\n";
    if (!output) {
        setError(errorMessage, "Failed while writing grid conversion options: " + location);
        return false;
    }
    return true;
}

} // namespace infrastructure
} // namespace next
} // namespace dev701
