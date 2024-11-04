#ifndef FILESYSTEM_H
#define FILESYSTEM_H


#ifdef _WIN32
#include <filesystem>
namespace fs = std::filesystem;
namespace filesystem = std::filesystem;
#elif __GNUC__ > 7
#include <filesystem>
namespace fs = std::filesystem;
namespace filesystem = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
namespace filesystem = std::experimental::filesystem;
#endif


#endif // FILESYSTEM_H
