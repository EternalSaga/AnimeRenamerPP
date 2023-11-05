// AnimeRenamer.cpp : Defines the entry point for the application.
//

#include "AnimeRenamer.h"
#include <ranges>
#include <nlohmann/json.hpp>
#include <fstream>
#include <algorithm>
#include <boost/locale.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex/icu.hpp>
using json = nlohmann::json;
namespace ranges = std::ranges;
namespace view = std::views;
const std::map<std::string, boost::regex> Resources::presetLanguageMap = {
		{"sc", boost::regex(R"(SC|sc|CHS|chs)")},
		{"tc",boost::regex(R"(TC|tc|CHT|cht)")},
		{"jap",boost::regex(R"(jap|jpn)")}
};
const std::string to_be_delete = "to_be_delete";
const std::vector<std::string> Resources::videoSuffix{ ".mp4", ".mkv", ".rmvb", ".avi", ".flv", ".mov", ".wmv", ".mpeg", ".m4v" };
const std::vector<std::string> Resources::subtitleSuffix{ ".srt",".ass",".sub",".ssa" };
//以[开头,以]结尾，且中括号都不包含，括号中间是1-3个数字
//以[开头,以]结尾，且中括号都不包含，括号中间是SP+多个数字
//1-3个数字
//S01E01格式
const boost::regex Resources::presetAnimeRegxes{
	R"((?<=\[)\d{1,3}(?<!\])|(?<=\[)(SP\d?)(?=\])|\b(\d{1,3})\b|(?<=S\d{2}E)(\d{2}))"
	//R"((?<=S\d{2}E)(\d{2}))"
};

AnimeRenamemer::AnimeRenamemer(const fs::path& animePath, const std::string& season, const std::string& animeName) :
	animeRootPath(animePath),
	season(season),
	animeName(animeName) {
	reg = Resources::presetAnimeRegxes;
}

std::string gbk2u8(const std::string gbkStr) {
	return boost::locale::conv::between(gbkStr, "UTF-8", "GBK");;
}

std::string u82gtk(const std::string u8Str) {
	return boost::locale::conv::between(u8Str, "GBK", "UTF-8");;
}

std::string filterSmatch2Episode(const boost::smatch& episode) {
	std::string result;
	for (const auto& re : episode)
	{
		if (re.matched)
		{
			result = re.str();
			break;
		}

	}
	if (episode.empty())
	{
		result = to_be_delete;
	}
	return result;
}

std::string preprocess(std::string s) {
	boost::erase_all(s, "1080P");
	boost::erase_all(s, "1080p");
	boost::erase_all(s, "720p");
	boost::erase_all(s, "720P");
	return s;
}

std::vector<std::pair<fs::path, fs::path>> AnimeRenamemer::getPreviewResult()
{
	if (animePaths_old_new.empty())
	{
		searchAnime();
		searchSubtitles();
	}
	checkResultValidation();
	std::vector<std::pair<fs::path, fs::path>> animeNameSubList;
	animeNameSubList.insert(animeNameSubList.end(), animePaths_old_new.begin(), animePaths_old_new.end());
	animeNameSubList.insert(animeNameSubList.end(), subtitlesPaths_old_new.begin(), subtitlesPaths_old_new.end());
	return animeNameSubList;
}

void AnimeRenamemer::setRegex(const std::string& regRule)
{
	if (regRule == "[集数]")
	{
		reg = boost::regex{ R"((?<=\[)\d{1,3}\.?5?(?=v??2??\]))" };
	}
	else if (regRule == "S季E集数") {
		reg = boost::regex{ R"((?<=S\d{2}E)(\d{2}))" };
	}
	else if (regRule == "集数") {
		//reg = boost::regex{ R"((?<=\s)\d{1,3}(?=\s)|(?<=第)\d{1,3}(?=话))" };\b\d{1,3}\b
		reg = boost::regex{ R"(\b\d{1,3}\b)" };
	}
	else {
		reg = Resources::presetAnimeRegxes;
	}
}
void AnimeRenamemer::searchAnime()
{
	const auto oldAnimeNames = GetAllFiles(
		this->animeRootPath, Resources::videoSuffix
	);
	auto animesPathView = oldAnimeNames | view::transform(
		[this](const fs::path& p) {
			std::string fileName;
			if (p.has_filename()) {
				fileName = preprocess( gbk2u8(p.filename().string()));
				//fileName = preprocess(p.filename().string());
			}
			boost::smatch episodes;
			boost::regex_search(fileName, episodes, reg);
			const auto ep = filterSmatch2Episode(episodes);

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
			std::string fileName;
			if (p.has_filename()) {
				fileName = preprocess(p.filename().string());
			}
			boost::smatch episodes;

			boost::regex_search(fileName, episodes, reg);
			const auto ep = filterSmatch2Episode(episodes);
			std::string languageCode;
			for (const auto& pair : Resources::presetLanguageMap)
			{
				boost::smatch foundLanguage;
				boost::regex_search(fileName, foundLanguage, pair.second);
				for (const auto& subm:foundLanguage)
				{
					if (subm.matched) {
						languageCode = pair.first;
						break;
					}
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
		auto u8oldPath = boost::locale::conv::between(path.first.string(), "UTF-8", "GBK");
		auto u8newPath = boost::locale::conv::between(path.second.string(), "UTF-8", "GBK");
		j["oldPath"].push_back(u8oldPath);
		j["newPath"].push_back(u8newPath);
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
			auto gbkOldPath = boost::locale::conv::between(data["oldPath"][i], "GBK", "UTF-8");
			auto gbkNewPath = boost::locale::conv::between(data["newPath"][i], "GBK", "UTF-8");


			fs::path oldPath(gbkOldPath);
			fs::path newPath(gbkNewPath);
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
