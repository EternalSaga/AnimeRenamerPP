// AnimeRenamer.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <regex>
#include <format>
#include "GetFiles.h"
#include <array>
#include <map>
#include <unicode/regex.h>
#include <fmt/format.h>
#include <cuchar>
using namespace std::string_literals;
struct Resources
{
	const static std::vector<std::string> videoSuffix;
	const static std::vector<std::string> subtitleSuffix;
	
	const static std::map<std::string, icu::UnicodeString> presetLanguageMap;
	//用于集数提取
	const static icu::UnicodeString presetAnimeRegxes;
private:
	
};

class AnimeRenamemer {
public:

	void doRename();
	void backUpPathes();
	void restoreBackup(const fs::path& animePath);

	void setRegex(const std::string& regRule);
	void setPath(fs::path videoPath);
	void setAnimeName(const std::string& animeName);
	void setSeason(const std::string& season);

	std::vector<std::pair<fs::path, fs::path>> getPreviewResult();
	AnimeRenamemer();
private:
	fs::path animeRootPath;
	std::string season;
	std::string animeName;
	void searchAnime();
	void searchSubtitles();
	void checkResultValidation() const;
	std::vector<std::pair<fs::path, fs::path>> animePaths_old_new;
	std::vector<std::pair<fs::path, fs::path>> subtitlesPaths_old_new;
	std::unique_ptr<icu::RegexMatcher> reg;

	UErrorCode icuStatus;
	
};
