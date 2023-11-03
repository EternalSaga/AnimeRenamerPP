#include "GetFiles.h"


std::vector<fs::path> GetAllFiles(const fs::path& path, const std::vector<std::string>& extensions){

    std::vector<fs::path> files;

    if (fs::exists(path) && fs::is_directory(path)){

        for (const auto& p : fs::directory_iterator (path)){

            if (fs::is_regular_file(p)){

                std::string ext = p.path().extension().string();

                if (std::find(extensions.begin(), extensions.end(), ext) != extensions.end()){
                    files.push_back(p.path());
                }
            }
        }
    }

    return files;
}
