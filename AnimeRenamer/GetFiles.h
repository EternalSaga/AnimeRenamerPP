#pragma once
#include <boost/filesystem.hpp>
#include <vector>
#include <string>
#include <boost/locale.hpp>
namespace fs = boost::filesystem;

std::vector<fs::path> GetAllFiles(const fs::path& path, const std::vector<std::string>& extensions);
