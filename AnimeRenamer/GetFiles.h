#pragma once
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

std::vector<fs::path> GetAllFiles(const fs::path& path, const std::vector<std::string>& extensions);
