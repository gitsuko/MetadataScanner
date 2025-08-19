#include <iostream>
#include <filesystem>
#include <chrono>
#include <iomanip>

namespace fs = std::filesystem;

int main() {
    std::cout << "Enter file name for scanning: " ;
    std::string path;
    std::cin >> path;

    try {
        if (fs::exists(path)) {
            std::cout << "Name: " << path << "\n";

            fs::path p = fs::absolute(path);
            std::cout << "Path: " << p.parent_path() << "\n";

            auto fstatus = fs::status(path);

            std::cout << "Type: "
                << (fstatus.type() == fs::file_type::regular ? "file" : "")
                << (fstatus.type() == fs::file_type::directory ? "directory" : "")
                << (fstatus.type() == fs::file_type::symlink ? "Symbolic link" : "")
                << "\n";

            if (fstatus.type() == fs::file_type::regular) { // if input is file
                std::cout << "Extension: " << p.extension() << "\n";

                std::uintmax_t file_size = fs::file_size(path); // lorg integer type

                if (file_size >= 1024 * 1024 * 1024) { // GB
                    double size_gb = static_cast<double>(file_size) / (1024 *1024 * 1024);
                    std::cout << "Size: " << std::fixed << std::setprecision(3) << size_gb << " GB\n";
                }
                if (file_size >= 1024 * 1024) { // MB
                    double size_mb = static_cast<double>(file_size) / (1024 * 1024);
                    std::cout << "Size: " << std::fixed << std::setprecision(3) << size_mb << " MB\n";
                } else if (file_size >= 1024) { // KB
                    double size_kb = static_cast<double>(file_size) / 1024;
                    std::cout << "Size: " << std::fixed << std::setprecision(3) << size_kb << " KB\n";
                } else { // Bytes
                    std::cout << "Size: " << file_size << " bytes\n";
                }
            }

            // Permissions (read, write, execute)
            std::cout << "Permissions: " 
            << ((fstatus.permissions() & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
            << ((fstatus.permissions() & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
            << ((fstatus.permissions() & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
            << "\n";

            // Last Modified Time
            auto ftime = fs::last_write_time(path);
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - fs::file_time_type::clock::now()
                + std::chrono::system_clock::now());
            
            std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
            std::cout << "Last Modified: " << std::asctime(std::localtime(&cftime)) << "\n";

        } else {
            std::cout << "File does not exist.\n";
        }

    } catch (fs::filesystem_error& e) {
        std::cerr << "Filesystem Error: " << e.what() << "\n";
    }

    return 0;
}