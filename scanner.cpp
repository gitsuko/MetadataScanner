#include "scanner.h"
#include <filesystem>
#include <chrono>
#include <iomanip>
namespace fs = std::filesystem;

std::vector<std::pair<std::string, std::string>> scanFile(const std::string &path) {
    std::vector<std::pair<std::string, std::string>> result;
    fs::path file(path);
    try {
        if (fs::exists(path)) {
            fs::path p = fs::absolute(path);
            result.push_back({"Root Path", p.parent_path()});

            auto fstatus = fs::status(path);
            std::string ftype;
            ftype = (fstatus.type() == fs::file_type::regular) ? "file" :
                        (fstatus.type() == fs::file_type::symlink) ? "symbolic link" :
                        "other";
            result.push_back({"Type", ftype});

            result.push_back({"Extention", p.extension()});

            std::uintmax_t file_size = fs::file_size(path);
            std::ostringstream ss;
            if (file_size >= 1024 * 1024 * 1024) { // GB
                double size_gb = static_cast<double>(file_size) / (1024 *1024 * 1024);
                ss << std::fixed << std::setprecision(2) << size_gb << " GB";
            } else if (file_size >= 1024 * 1024) { // MB
                double size_mb = static_cast<double>(file_size) / (1024 * 1024);
                ss << std::fixed << std::setprecision(2) << size_mb << " MB";
            } else if (file_size >= 1024) { // KB
                double size_kb = static_cast<double>(file_size) / 1024;
                ss << std::fixed << std::setprecision(2) << size_kb << " KB";
            } else { // Bytes
                ss << file_size << " bytes";
            }
            result.push_back({"Size", ss.str()});

            std::string permissions;
            permissions += ((fstatus.permissions() & fs::perms::owner_read) != fs::perms::none ? "r" : "-");
            permissions += ((fstatus.permissions() & fs::perms::owner_write) != fs::perms::none ? "w" : "-");
            permissions += ((fstatus.permissions() & fs::perms::owner_exec) != fs::perms::none ? "x" : "-");
            result.push_back({"Permissions", permissions});

            auto ftime = fs::last_write_time(path);
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration> (
                ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
            std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
            result.push_back({"Last Modified", std::asctime(std::localtime(&cftime))});

        } else {
            result.push_back({"Not Found", "File not found"});
            return result;
        }
    } catch (fs::filesystem_error& e) {

        result.push_back({"Error", e.what()});
        return result;
    }

    return result;
}
