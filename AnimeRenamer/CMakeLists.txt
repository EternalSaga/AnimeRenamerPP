# CMakeList.txt : CMake project for AnimeRenamer, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (AnimeRenamer WIN32 "AnimeRenamer.cpp" "AnimeRenamer.h" "GetFiles.h" "GetFiles.cpp" "main.cpp" "AppGui.h" "AppGui.cpp")


find_package(spdlog CONFIG REQUIRED)
target_link_libraries(AnimeRenamer PRIVATE spdlog::spdlog)
find_package(Boost REQUIRED COMPONENTS filesystem regex locale)

target_link_libraries(AnimeRenamer PRIVATE Boost::filesystem Boost::regex Boost::locale)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(AnimeRenamer PRIVATE fmt::fmt)

find_package(wxWidgets CONFIG REQUIRED)
target_link_libraries(AnimeRenamer PRIVATE wx::core wx::base)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(AnimeRenamer PRIVATE nlohmann_json::nlohmann_json)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET AnimeRenamer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
