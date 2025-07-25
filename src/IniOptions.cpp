#include "IniOptions.h"

#include "AnimalInfo.h"
#include "Categories.h"
#include "CategoryInfo.h"
#include "IContainingInfo.h"
#include "IInfo.h"
#include "QualityFilter.h"

#include "SimpleIni/SimpleIni.h"

#include <spdlog/spdlog.h>

#include <map>
#include <memory>
#include <processenv.h>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <Windows.h>

char const* const IniOptions::generalSectionName{ "GENERAL" };
char const* const IniOptions::languageSectionName{ "LANG" };
char const* const IniOptions::animalSectionName{ "ANIMAL_RARITY" };
char const* const IniOptions::categorySectionName{ "CATEGORY_RARITY" };

IniOptions::IniOptions(std::string const & generalInifile)
{
	if (m_generalIni.LoadFile(generalInifile.c_str()) < 0) {
		throw std::runtime_error{ "Error! Failed to load the ini file: " + generalInifile};
	}

	m_langFilePath = ExpandEnvironmentVariables(m_generalIni.GetValue(generalSectionName, "langFilePath", "eng.ini"));
	m_stateFilePath = ExpandEnvironmentVariables(m_generalIni.GetValue(generalSectionName, "stateFilePath", "AnimalTracker/state.ini"));
}

std::vector<std::unique_ptr<IInfo>> IniOptions::LoadInfo() const
{
	std::vector<std::unique_ptr<IInfo>> returnVector{};
	std::vector<const IInfo*> storeVector{}; // to write back result in the end

	CSimpleIniA langIni;
	if (langIni.LoadFile(m_langFilePath.c_str()) < 0) {
		spdlog::error("Failed to load the language file {}", m_langFilePath);
		throw std::runtime_error{ std::string{ "Error! Failed to load the language file: " } + m_langFilePath };
	};

	CSimpleIniA stateIni;
	if(stateIni.LoadFile(m_stateFilePath.c_str()) < 0) {// error is fine, just use defaults instead
		spdlog::info("No savefile found under {}. Creating new savefile after init", m_stateFilePath);
	}

	std::map<RootCategory, IContainingInfo*> tempRootMap;
	for (RootCategory const& category : ROOT_CATEGORIES) {
		std::string name{ langIni.GetValue(languageSectionName, category._to_string(), category._to_string()) };
		spdlog::debug("Read key {} in section {} in language file {}: {}", category._to_string(), languageSectionName, m_langFilePath, name);
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
		spdlog::debug("Read key {} in section {} in language file {}: {}", sub_root_pair.first._to_string(), languageSectionName, m_langFilePath, name);
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
		spdlog::debug("Read key {} in section {} in language file {}: {}", animal_sub_pair.first._to_string(), languageSectionName, m_langFilePath, name);
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

void IniOptions::StoreInfos(std::vector<const IInfo*> infos) const
{
	CSimpleIniA stateIni;
	if (stateIni.LoadFile(m_stateFilePath.c_str()) < 0) {
		spdlog::info("No savefile found under {}. Creating new savefile...", m_stateFilePath);
	}; // error is fine, just write instead
	for (auto const& entry : infos) {
		stateIni.SetLongValue(
			entry->GetClass() == InfoClass::AnimalInfo ? animalSectionName : categorySectionName,
			entry->GetKey().c_str(),
			static_cast<long>(entry->GetQualityBitmask())
		);
	}
	if (stateIni.SaveFile(m_stateFilePath.c_str()) < 0) {
		spdlog::warn("Unable to write to file {}! Check your file priviliges or configure a different output file.", m_stateFilePath);
	}
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