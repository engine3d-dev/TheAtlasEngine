#pragma once
#include <string>
#include <filesystem>

namespace atlas::filesystem {
    /**
     * @param filter
     * @note Defines the filter of items to pick from the file dialog
     * @example passing "obj;jpg" will only allow to select these files with
     * those extensions
     */
    std::string load_from_file_dialog(
      const std::string& p_filter,
      const std::filesystem::path& p_default_path = "Untitled.engine");

    std::string save_to_file(
      const std::string& p_filter,
      const std::filesystem::path& p_default_path = "Untitled.engine");
}; // namespace atlas::filesystem