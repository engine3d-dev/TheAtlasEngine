#pragma once
#include <string>

namespace atlas::filesystem {
    /**
     * @param filter
     * @note Defines the filter of items to pick from the file dialog
     * @example passing "obj;jpg" will only allow to select these files with
     * those extensions
     */
    std::string LoadFromFileDialog(const std::string& p_filter);

    std::string SaveToFile(const std::string& p_filter);
}; // namespace atlas::filesystem