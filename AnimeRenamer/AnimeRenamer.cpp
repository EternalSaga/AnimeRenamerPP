// AnimeRenamer.cpp : Defines the entry point for the application.
//

#include "AnimeRenamer.h"
#include <ranges>
#include <nlohmann/json.hpp>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <set>

using json = nlohmann::json;
namespace ranges = std::ranges;
namespace view = std::views;
const std::map<std::string, icu::UnicodeString> Resources::presetLanguageMap = {
		{"sc", R"(SC|sc|CHS|chs)"},
		{"tc",R"(TC|tc|CHT|cht)"},
		{"jap",R"(jap|jpn)"}
};
const std::string to_be_delete = "to_be_delete";
const std::vector<std::string> Resources::videoSuffix{ ".mp4", ".mkv", ".rmvb", ".avi", ".flv", ".mov", ".wmv", ".mpeg", ".m4v" };
const std::vector<std::string> Resources::subtitleSuffix{ ".srt",".ass",".sub",".ssa" };
//以[开头,以]结尾，且中括号都不包含，括号中间是1-3个数字
//以[开头,以]结尾，且中括号都不包含，括号中间是SP+多个数字
//1-3个数字
//S01E01格式
const icu::UnicodeString Resources::presetAnimeRegxes = R"((?<=\[)\d{1,3}(?<!\])|(?<=\[)(SP\d?)(?=\])|\b(\d{1,3})\b|(?<=S\d{2}E)(\d{2}))"
	//R"((?<=S\d{2}E)(\d{2}))"
;

AnimeRenamemer::AnimeRenamemer(const fs::path& animePath, const std::string& season, const std::string& animeName) :
	animeRootPath(animePath),
	season(season),
	animeName(animeName) {
	reg = nullptr;
}
icu::UnicodeString u8str2icu(const std::u8string& str) {
	return icu::UnicodeString::fromUTF8(std::string(str.begin(), str.end()));
}
std::string findFirstMatch(const icu::UnicodeString& ustrNeedMatch,icu::RegexMatcher* matcher) {
	UErrorCode status = U_ZERO_ERROR;
	std::string u8str;
	matcher->reset(ustrNeedMatch);
	if (matcher->find()) {
		int start = matcher->start(status);
		if (U_FAILURE(status))
		{
			throw status;
		}
		int end = matcher->end(status);
		if (U_FAILURE(status))
		{
			throw status;
		}
		icu::UnicodeString m;
		ustrNeedMatch.extractBetween(start, end, m);
		
		m.toUTF8String(u8str);
		
	}
	return u8str;
}


std::vector<std::pair<fs::path, fs::path>> AnimeRenamemer::getPreviewResult()
{
	if (animePaths_old_new.empty())
	{
		searchAnime();
		searchSubtitles();
	}
	checkResultValidation();
	std::vector<std::pair<fs::path, fs::path>> animeNameSubList = animePaths_old_new;
	animeNameSubList.insert(animeNameSubList.end(), subtitlesPaths_old_new.begin(), subtitlesPaths_old_new.end());
	return animeNameSubList;
}

void AnimeRenamemer::setRegex(const std::string& regRule)
{
	if (regRule == "[集数]")
	{
		reg = std::make_unique<icu::RegexMatcher>( R"((?<=\[)\d{1,3}\.?5?(?=v??2??\])|(?<=\[)SP\d?\d?(?=v??2??\]))",0,this->icuStatus);
	}
	else if (regRule == "S季E集数") {
		reg = std::make_unique<icu::RegexMatcher>( R"((?<=S\d{2}E)(\d{2}))",0, this->icuStatus);
	}
	else if (regRule == "集数") {

		reg = std::make_unique<icu::RegexMatcher>( R"((?<=第)\d{1,3}(?=话)|(?<=\s)\d{1,3}(?=\s)|\b\d{1,3}\b)",0, this->icuStatus);
	}
	else {
		reg = std::make_unique<icu::RegexMatcher>(Resources::presetAnimeRegxes,0, this->icuStatus);
	}
	if (U_FAILURE(icuStatus))
	{
		throw icuStatus;
	}
}
void AnimeRenamemer::searchAnime()
{
	const auto oldAnimeNames = GetAllFiles(
		this->animeRootPath, Resources::videoSuffix
	);
	auto animesPathView = oldAnimeNames | view::transform(
		[this](const fs::path& p) {
			std::u8string fileName;
			if (p.has_filename()) {
				fileName = p.filename().u8string();				//fileName = preprocess(p.filename().string());
			}

			const auto icuFilename = u8str2icu(fileName);
			
			const auto ep = findFirstMatch(icuFilename,reg.get());

			const auto newFileName = fmt::format("{}-S{}E{}{}", animeName, season, ep, p.extension().string());
			const auto newFilePath = p.parent_path() / fs::path(newFileName);
			return std::pair<fs::path, fs::path>{p, newFilePath};
		}) | view::filter([](const std::pair<fs::path, fs::path>& old_new) {
				return old_new.second.string().find(to_be_delete);
			});
		for (const auto& v : animesPathView)
		{
			animePaths_old_new.push_back(v);
		}
}



void AnimeRenamemer::searchSubtitles()
{
	const auto oldSubtitleNames = GetAllFiles(this->animeRootPath, Resources::subtitleSuffix);
	auto subtitlePathView = oldSubtitleNames | view::transform(
		[this](const fs::path& p) {
			std::u8string fileName;
			if (p.has_filename()) {
				fileName = p.filename().u8string();
			}
			const auto icuFilename = u8str2icu(fileName);

			const auto ep = findFirstMatch(icuFilename, reg.get());

			std::string languageCode;
			for (auto pair : Resources::presetLanguageMap)
			{
				icu::RegexMatcher matcher(pair.second, icuFilename,0, this->icuStatus);
				if (U_FAILURE(icuStatus))
				{
					throw icuStatus;
				}
				if (matcher.find())
				{
					languageCode = pair.first;
					break;
				}
			}
			const auto newFileName = fmt::format("{}-S{}E{}.{}{}", animeName, season, ep, languageCode, p.extension().string());
			const auto newFilePath = p.parent_path() / fs::path(newFileName);
			return std::pair<fs::path, fs::path>{p, newFilePath};
		}
	);
	for (const auto& v : subtitlePathView)
	{
		subtitlesPaths_old_new.push_back(v);
	}
}

void AnimeRenamemer::checkResultValidation() const
{
	if (animePaths_old_new.size() == 0) {
		throw std::runtime_error("动画集数匹配失败，请检查正则表达式");
	}
	std::set<std::string> animeNameSet;
	int previewsSize = 0;
	for (const auto animeNames : animePaths_old_new)
	{
		animeNameSet.emplace(animeNames.second.filename().generic_string());
		if (previewsSize == animeNameSet.size())
		{

			throw std::runtime_error(fmt::format("新动画名有重名，请检查动画名：{}:{}", animeNames.first.filename().generic_string(), animeNames.second.filename().generic_string()));
		}
		previewsSize = animeNameSet.size();
	}

	std::set<std::string> subtileSet;
	previewsSize = 0;
	for (const auto subtitleName : subtitlesPaths_old_new) {
		subtileSet.emplace(subtitleName.second.filename().generic_string());
		if (previewsSize == subtileSet.size()) {
			throw std::runtime_error(fmt::format("新字幕有重名，请检查字幕名：{}:{}", subtitleName.first.filename().generic_string(), subtitleName.second.filename().generic_string()));
		}
		previewsSize = subtileSet.size();
	}

}


void AnimeRenamemer::doRename() {
	for (const auto& animes : animePaths_old_new) {
		fs::rename(animes.first, animes.second);
	}
	for (const auto& subs : subtitlesPaths_old_new)
	{
		fs::rename(subs.first, subs.second);
	}
}


void AnimeRenamemer::backUpPathes() {
	const auto pathes = AnimeRenamemer::getPreviewResult();
	json j;
	fs::path writePath = pathes[0].first.parent_path() / "backup.json";


	for (const auto& path : pathes)
	{
		const auto u8old = path.first.u8string();
		const auto u8new = path.second.u8string();
		j["oldPath"].push_back(std::string(u8old.begin(), u8old.end()));
		j["newPath"].push_back(std::string(u8new.begin(), u8new.end()));
	}
	const std::string s = j.dump();
	std::fstream f(writePath.string(), std::ios::out | std::ios::trunc);//只写打开，抛弃所有文件
	f.write(s.c_str(), s.size());
}

void AnimeRenamemer::restoreBackup(const fs::path& animePath) {





	const fs::path backupPath = animePath / "backup.json";
	std::ifstream ifs(backupPath.string());
	if (ifs.fail())
	{
		return;
	}
	try {
		json data = json::parse(ifs);

		std::map<std::string, std::vector<std::string>> map = data.get<std::map<std::string, std::vector<std::string>>>();
		for (size_t i = 0; i < data["oldPath"].size(); i++)
		{
			std::string oldPathStr = data["oldPath"][i];
			std::string newPathStr = data["newPath"][i];
			std::u8string oldu8(oldPathStr.begin(), oldPathStr.end());
			std::u8string newu8(newPathStr.begin(), newPathStr.end());
			fs::path oldPath(oldu8);
			fs::path newPath(newu8);
			oldPath = oldPath.make_preferred();
			newPath = newPath.make_preferred();
			if (fs::exists(newPath))
			{
				fs::rename(newPath, oldPath);
			}

		}
	}
	catch (json::parse_error& e) {
		std::cout << e.what() << std::endl;
	}
	catch (json::exception& e) {
		std::cout << e.what() << std::endl;
	}
}
