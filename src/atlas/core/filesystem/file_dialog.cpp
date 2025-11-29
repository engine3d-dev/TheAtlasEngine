#include <core/engine_logger.hpp>
#include <core/filesystem/file_dialog.hpp>
#include <nfd/nfd.h>

namespace atlas::filesystem {
    std::string load_from_file_dialog(
      const std::string& p_filter,
      const std::filesystem::path& p_default_path) {
        char* output_path = nullptr;

        nfdresult_t result = NFD_OpenDialog(
          p_filter.c_str(), p_default_path.string().c_str(), &output_path);

        // if (result == NFD_OKAY) {
        //     return std::string(output_path);
        // }

        // console_log_fatal("Error Loading File!");
        // return "";
        return (result == NFD_OKAY) ? std::string(output_path) : "";
    }

    std::string save_to_file(const std::string& p_filter,
                             const std::filesystem::path& p_default) {
        char* output_path = nullptr;
        nfdresult_t result = NFD_SaveDialog(
          p_filter.c_str(), p_default.string().c_str(), &output_path);

        // if(result == NFD_ERROR){
        // 	return std::string();
        // }

        // return std::string(output_path);

        return (result == NFD_OKAY) ? std::string(output_path) : std::string();
    }
};