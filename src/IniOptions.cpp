#include "IniOptions.h"

#include "AnimalInfo.h"
#include "ButtonMapping.h"
#include "Categories.h"
#include "CategoryInfo.h"
#include "IContainingInfo.h"
#include "IInfo.h"
#include "InputAction.h"
#include "QualityFilter.h"

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

IniOptions::IniOptions(std::filesystem::path const & generalInifile):
	m_generalIni{ true, true },
	m_locale{ "en" }
{
	if (m_generalIni.LoadFile(generalInifile.c_str()) < 0) {
		throw std::runtime_error{ ("Error! Failed to load the ini file: " + generalInifile.string())};
	}

	m_langFilePath = ExpandEnvironmentVariables(m_generalIni.GetValue(generalSectionName, "langFilePath", "en.ini"));
	m_stateFilePath = ExpandEnvironmentVariables(m_generalIni.GetValue(generalSectionName, "stateFilePath", "AnimalTracker/state.ini"));

	CSimpleIniA langIni{ true };
	if (langIni.LoadFile(m_langFilePath.c_str()) < 0) {
		spdlog::error("Failed to load the language file {}", m_langFilePath.string());
		return;
	};
	m_locale = langIni.GetValue(generalSectionName, "LOCALE", "en");
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
		spdlog::error("{} section missing in general ini", buttonMappingsSectionName);
		throw std::runtime_error{ std::string{ "Controls Section missing in ini" } };
	}
	for (CSimpleIniA::Entry const& key : keyList) {

		bool withControl{ false };
		bool withAlt{ false };
		bool withShift{ false };
		std::string keyString{ key.pItem };
		for (;;) {
			if (keyString.starts_with(withControlPrefix)) {
				withControl = true;
				keyString = keyString.substr(std::string_view{ withControlPrefix }.size());
				continue;
			}
			if (keyString.starts_with(withAltPrefix)) {
				withAlt = true;
				keyString = keyString.substr(std::string_view{ withAltPrefix }.size());
				continue;
			}
			if (keyString.starts_with(withShiftPrefix)) {
				withShift = true;
				keyString = keyString.substr(std::string_view{ withShiftPrefix }.size());
				continue;
			}
			break;
		}

		better_enums::optional<InputAction> action = InputAction::_from_string_nothrow(keyString.c_str());
		if (!action) {
			static auto inputNames{ InputAction::_names() };
			spdlog::warn("Unkown control instruction in ini file: {}. Valid values are: {}", key.pItem, fmt::join(inputNames, ", "));
			continue;
		}

		std::list<long> valueList{};
		if (!m_generalIni.GetAllLongValues(buttonMappingsSectionName, key.pItem, valueList, -1)) {
			spdlog::error("Unable to retrieve items of key {} in section {} of the general ini", buttonMappingsSectionName, key.pItem);
			throw std::runtime_error{ std::string{ "Unable to read values from ini" } };
		}
		
		for (long const& value : valueList) {
			if (value < 0x01 || value > 0xFF) {
				spdlog::warn("Invalid Key value in ini for key \"{}\": {}", key.pItem, value);
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
	CSimpleIniA stateIni;
	if (stateIni.LoadFile(m_stateFilePath.c_str()) < 0) {
		spdlog::info("No savefile found under {}. Creating new savefile...", m_stateFilePath.string());
	}; // error is fine, just write instead
	for (auto const& entry : infos) {
		stateIni.SetLongValue(
			entry->GetClass() == InfoClass::AnimalInfo ? animalSectionName : categorySectionName,
			entry->GetKey().c_str(),
			static_cast<long>(entry->GetQualityBitmask())
		);
	}
	if (!std::filesystem::exists(m_stateFilePath.parent_path())) {
		std::filesystem::create_directories(m_stateFilePath.parent_path());
	}
	if (stateIni.SaveFile(m_stateFilePath.c_str()) < 0) {
		spdlog::warn("Unable to write to file {}! Check your file priviliges or configure a different output file.", m_stateFilePath.string());
	}
}

const char* IniOptions::GetLocale() const
{
	return m_locale.c_str();
}

std::string IniOptions::ExpandEnvironmentVariables(const std::string& input)
{
	DWORD requiredSize = ExpandEnvironmentStringsA(input.c_str(), nullptr, 0);
	if (requiredSize == 0) { //expansion failure, return original
		return input;
	}
	std::vector<char>buffer(static_cast<size_t>(requiredSize), '\0');
	DWORD charsWritten = ExpandEnvironmentStringsA(input.c_str(), buffer.data(), requiredSize);
	if (charsWritten == 0 || charsWritten > requiredSize) {
		return input;
	}
	return std::string{ buffer.begin(), buffer.end() };
}