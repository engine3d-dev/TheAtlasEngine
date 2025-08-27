#include <core/engine_logger.hpp>
#include <core/filesystem/file_dialog.hpp>
#include <nfd/nfd.h>

namespace atlas::filesystem {
    std::string load_from_file_dialog(const std::string& p_filter) {
        char* output_path = nullptr;

        nfdresult_t result =
          NFD_OpenDialog(p_filter.c_str(), nullptr, &output_path);

        if (result == NFD_OKAY) {
            return std::string(output_path);
        }

        console_log_fatal("Error Loading File!");
        return "";
    }

    std::string save_to_file([[maybe_unused]] const std::string& p_filter) {
        char* output_path = nullptr;

        nfdresult_t result =
          NFD_SaveDialog(p_filter.c_str(), nullptr, &output_path);

        if (result == NFD_OKAY) {
            return std::string(output_path);
        }

        console_log_trace("Error Loading == {}", output_path);
        return "";
    }
};