#include "IniOptions.h"

#include "AnimalInfo.h"
#include "ButtonMapping.h"
#include "Categories.h"
#include "CategoryInfo.h"
#include "IContainingInfo.h"
#include "IInfo.h"
#include "InputAction.h"
#include "QualityFilter.h"

#include <ScriptHookRDR2/natives.h>

#include <SimpleIni/SimpleIni.h>

#include <spdlog/spdlog.h>

#include <fmt/ranges.h> 

#include <better-enums/enum.h>

#include <filesystem>
#include <list>
#include <map>
#include <memory>
#include <processenv.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <Windows.h>

char const* const IniOptions::generalSectionName{ "GENERAL" };
char const* const IniOptions::languageSectionName{ "LANG" };
char const* const IniOptions::animalSectionName{ "ANIMAL_RARITY" };
char const* const IniOptions::categorySectionName{ "CATEGORY_RARITY" };
char const* const IniOptions::buttonMappingsSectionName{ "CONTROLS" };
char const* const IniOptions::withControlPrefix{"CTRL_"};
char const* const IniOptions::withAltPrefix{ "ALT_" };
char const* const IniOptions::withShiftPrefix{ "SHIFT_" };

std::map<int, std::string> const LANGUAGE_MAP{ 
	{0, "en"}, 
	{1, "fr"}, 
	{2, "de"}, 
	{3, "it"},
	{4, "es"},
	{5, "pt"},
	{6, "pl"},
	{7, "ru"},
	{8, "ko"},
	{9, "cn"},	// taiwanese, same as simplified??
	{10, "ja"},
	{11, "es"},	// mexican, same as regular spanish?
	{12, "cn"}
};

IniOptions::IniOptions(std::filesystem::path const & generalInifile):
	m_generalIni{ true, true },
	m_locale{ "en" }
{
	if (m_generalIni.LoadFile(generalInifile.c_str()) < 0) {
		std::runtime_error exception{ ("Failed to load the ini file: " + generalInifile.string()) };
		spdlog::critical("{}", exception.what());
		throw exception;
	}

	std::string logLevelString = m_generalIni.GetValue(generalSectionName, "logLevel", "");
	auto logLevel = spdlog::level::from_str(logLevelString);
	if (logLevelString.empty() || ((logLevel == spdlog::level::off) && (logLevelString != "off"))) {
		spdlog::set_level(spdlog::level::info);
		spdlog::flush_on(spdlog::level::info);
		spdlog::warn("\"logLevel\" in ini file {} is either not present or contains invalid value \"{}\". Using default Info loglevel.", generalInifile.string(), logLevelString);
	}
	else {
		spdlog::info("Setting loglevel to \"{}\"", spdlog::level::to_string_view(logLevel));
		spdlog::set_level(logLevel);
		spdlog::flush_on(logLevel); 
	}

	// legacy behaviour, allows users to inject their custom translations
	if (std::string langFile{ m_generalIni.GetValue(generalSectionName, "langFilePath", "") }; !langFile.empty()) {
		spdlog::info("User provided language file path detected, overwriting automatic language detection. {}", langFile);
		m_langFilePath = langFile;
	}
	else{
		auto it{ LANGUAGE_MAP.find(LOCALIZATION::GET_CURRENT_LANGUAGE()) };
		std::string const& langCode{ it == LANGUAGE_MAP.end() ? "en" : it->second };
		spdlog::info("Detected language \"{}\"", it->second);
		std::filesystem::path langFilePath = std::filesystem::path{ "AnimalTracker" } /  "Langs" / (langCode + ".ini");
		if (!std::filesystem::exists(langFilePath)) {
			m_langFilePath = langFilePath.parent_path() / "en.ini";
			spdlog::warn("No translation file found for language code \"{}\". Fallback to english translation with file {}", langCode, m_langFilePath.string());
		}
		else {
			m_langFilePath = std::move(langFilePath);
			spdlog::debug("Using language file {}", m_langFilePath.string());
		}
	}
	m_stateFilePath = ExpandEnvironmentVariables(m_generalIni.GetValue(generalSectionName, "stateFilePath", "AnimalTracker/state.ini"));
	spdlog::debug("State file configured: {}", m_stateFilePath.string());

	CSimpleIniA langIni{ true };
	if (langIni.LoadFile(m_langFilePath.c_str()) < 0) {
		spdlog::error("Failed to load the language file {}", m_langFilePath.string());
		return;
	};
	m_locale = langIni.GetValue(generalSectionName, "LOCALE", "en");
	spdlog::trace("Using locale {} for collation", m_locale);
}

std::vector<std::unique_ptr<IInfo>> IniOptions::LoadInfo() const
{
	std::vector<std::unique_ptr<IInfo>> returnVector{};
	std::vector<const IInfo*> storeVector{}; // to write back result in the end

	CSimpleIniA langIni{ true };
	if (langIni.LoadFile(m_langFilePath.c_str()) < 0) {
		spdlog::error("Failed to load the language file {}", m_langFilePath.string());
		throw std::runtime_error{ std::string{ "Error! Failed to load the language file: " } + m_langFilePath.string()};
	};

	CSimpleIniA stateIni;
	if(stateIni.LoadFile(m_stateFilePath.c_str()) < 0) {// error is fine, just use defaults instead
		spdlog::info("No savefile found under {}. Creating new savefile after init", m_stateFilePath.string());
	}

	std::map<RootCategory, IContainingInfo*> tempRootMap;
	for (RootCategory const& category : ROOT_CATEGORIES) {
		std::string name{ langIni.GetValue(languageSectionName, category._to_string(), category._to_string()) };
		spdlog::trace("Read key {} in section {} in language file {}: {}", category._to_string(), languageSectionName, m_langFilePath.string(), name);
		auto ptr{ std::make_unique<CategoryInfo>(
			category._to_string(),
			name,
			stateIni.GetLongValue(
				categorySectionName,
				category._to_string(),
				QualityFilter::NOT_SET
			),
			*this
		) };
		tempRootMap[category] = ptr.get();
		storeVector.push_back(ptr.get());
		returnVector.emplace_back(std::move(ptr));
	}

	std::map<SubCategory, IContainingInfo*> tempSubMap;
	for (auto const& sub_root_pair : SUB_CATEGORIES) {
		std::string name{ langIni.GetValue(languageSectionName, sub_root_pair.first._to_string(), sub_root_pair.first._to_string()) };
		spdlog::trace("Read key {} in section {} in language file {}: {}", sub_root_pair.first._to_string(), languageSectionName, m_langFilePath.string(), name);
		auto ptr{ std::make_unique<CategoryInfo>(
			sub_root_pair.first._to_string(),
			name,
			stateIni.GetLongValue(
				categorySectionName,
				sub_root_pair.first._to_string(),
				QualityFilter::NOT_SET
			),
			*this
		)};
		tempSubMap[sub_root_pair.first] = ptr.get();
		storeVector.push_back(ptr.get());
		tempRootMap.at(sub_root_pair.second)->AddContainedItem(std::move(ptr));
	}


	for (auto const& animal_sub_pair : ANIMALS) {
		std::string name{ langIni.GetValue(languageSectionName, animal_sub_pair.first._to_string(), animal_sub_pair.first._to_string()) };
		spdlog::trace("Read key {} in section {} in language file {}: {}", animal_sub_pair.first._to_string(), languageSectionName, m_langFilePath.string(), name);
		auto ptr{ std::make_unique<AnimalInfo>(
			animal_sub_pair.first,
			name,
			(SUB_CATEGORIES.at(animal_sub_pair.second)._value == RootCategory::CA_FISH),  // TODO get this somehow
			stateIni.GetLongValue(
				animalSectionName,
				std::to_string(animal_sub_pair.first._value).c_str(),
				QualityFilter::PERFECT
			),
			*this
		) };
		storeVector.push_back(ptr.get());
		tempSubMap.at(animal_sub_pair.second)->AddContainedItem(std::move(ptr));
	}

	StoreInfos(storeVector);
	return returnVector;
}

std::vector<ButtonMapping> IniOptions::LoadButtonMappings() const
{
	std::vector<ButtonMapping> returnVector{};
	std::list<CSimpleIniA::Entry> keyList{};
	if (!m_generalIni.GetAllKeys(buttonMappingsSectionName, keyList)) {
		spdlog::critical("{} section missing in general ini", buttonMappingsSectionName);
		throw std::runtime_error{ std::string{ "Controls Section missing in ini" } };
	}
	for (CSimpleIniA::Entry const& key : keyList) {

		bool withControl{ false };
		bool withAlt{ false };
		bool withShift{ false };
		std::string keyString{ key.pItem };
		for (;;) {
			if (keyString.starts_with(withControlPrefix)) {
				spdlog::trace("Control prefix in {}", keyString);
				withControl = true;
				keyString = keyString.substr(std::string_view{ withControlPrefix }.size());
				continue;
			}
			if (keyString.starts_with(withAltPrefix)) {
				spdlog::trace("Alt prefix in {}", keyString);
				withAlt = true;
				keyString = keyString.substr(std::string_view{ withAltPrefix }.size());
				continue;
			}
			if (keyString.starts_with(withShiftPrefix)) {
				spdlog::trace("Shift prefix in {}", keyString);
				withShift = true;
				keyString = keyString.substr(std::string_view{ withShiftPrefix }.size());
				continue;
			}
			break;
		}

		better_enums::optional<InputAction> action = InputAction::_from_string_nothrow(keyString.c_str());
		if (!action) {
			static auto inputNames{ InputAction::_names() };
			spdlog::error("Unkown control instruction in ini file: {}. Valid values are: {}", key.pItem, fmt::join(inputNames, ", "));
			continue;
		}

		std::list<long> valueList{};
		if (!m_generalIni.GetAllLongValues(buttonMappingsSectionName, key.pItem, valueList, -1)) {
			spdlog::critical("Unable to retrieve items of key {} in section {} of the general ini", buttonMappingsSectionName, key.pItem);
			throw std::runtime_error{ std::string{ "Unable to read values from ini" } };
		}
		
		for (long const& value : valueList) {
			if (value < 0x01 || value > 0xFF) {
				spdlog::error("Invalid Key value in ini for key \"{}\": {}", key.pItem, value);
				continue;
			}
			returnVector.push_back(ButtonMapping{ 
				.action = action.value(), 
				.keyCode = static_cast<unsigned int>(value), 
				.withCtrl = withControl, 
				.withAlt = withAlt,
				.withShift = withShift
			});
		}
	}
	return returnVector;
}

void IniOptions::StoreInfos(std::vector<const IInfo*> infos) const
{
	spdlog::debug("Writing {} entries to savefile", infos.size());
	CSimpleIniA stateIni;
	if (stateIni.LoadFile(m_stateFilePath.c_str()) < 0) {
		spdlog::info("No savefile found under {}. Creating new savefile upon writing...", m_stateFilePath.string());
	}; // error on reading is fine, just write later

	for (auto const& entry : infos) {

		const char* sectionName{ (entry->GetClass() == InfoClass::AnimalInfo) ? animalSectionName : categorySectionName };
		const char* keyName{ entry->GetKey().c_str() };
		long bitmask{ static_cast<long>(entry->GetQualityBitmask()) };

		spdlog::trace("Writing entry \"{}:{} = {}\" to savefile", sectionName, keyName, bitmask);
		stateIni.SetLongValue(sectionName, keyName, bitmask);
	}

	if (std::filesystem::path folderPath{ m_stateFilePath.parent_path() }; !std::filesystem::exists(folderPath)) {
		spdlog::info("Savefile directory {} does not exist, creating it", folderPath.string());
		std::filesystem::create_directories(folderPath);
	}
	if (stateIni.SaveFile(m_stateFilePath.c_str()) < 0) {
		spdlog::error("Unable to write to file {}! Check your file priviliges or configure a different output file.", m_stateFilePath.string());
		return;
	}
	spdlog::debug("Successfully wrote to savefile.");
}

const char* IniOptions::GetLocale() const
{
	return m_locale.c_str();
}

std::string IniOptions::ExpandEnvironmentVariables(const std::string& input)
{
	DWORD requiredSize = ExpandEnvironmentStringsA(input.c_str(), nullptr, 0);
	if (requiredSize == 0) { //expansion failure, return original
		spdlog::error("Failed to expand environment variables in string \"{}\"", input);
		return input;
	}
	std::vector<char>buffer(static_cast<size_t>(requiredSize), '\0');
	DWORD charsWritten = ExpandEnvironmentStringsA(input.c_str(), buffer.data(), requiredSize);
	if (charsWritten == 0 || charsWritten > requiredSize) {
		spdlog::error("Failed to expand environment variables in string \"{}\"", input);
		return input;
	}
	return std::string{ buffer.begin(), buffer.end() };
}