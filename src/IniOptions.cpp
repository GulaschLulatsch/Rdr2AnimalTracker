#include "IniOptions.h"

#include "AnimalInfo.h"
#include "NamesInfo.h"
#include "QualityFilter.h"

#include "RDR2ScriptHook/types.h"

#include "SimpleIni/SimpleIni.h"

#include <processenv.h>

#include <stdexcept>
#include <string>
#include <unordered_map>
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

std::unordered_map<Hash, AnimalInfo> IniOptions::GetAnimalMap() const
{

	CSimpleIniA langIni;
	if(langIni.LoadFile(m_langFilePath.c_str()) < 0) {
		throw std::runtime_error{ std::string{ "Error! Failed to load the language file: " } + m_langFilePath};
	};

	// Get all keys in a section
	CSimpleIniA::TNamesDepend keys;
	langIni.GetAllKeys(languageSectionName, keys);

	CSimpleIniA stateIni;
	stateIni.LoadFile(m_stateFilePath.c_str()); // error is fine, just use defaults instead

	std::unordered_map<Hash, AnimalInfo> animalInfos;

	// Iterate through keys and get their values
	for (const auto& key : keys) {
		const char* value = langIni.GetValue(languageSectionName, key.pItem, "undefinded");

		Hash animalHash = typetoHash.at(key.pItem);
		QualityFilter qualityFilter{
			static_cast<int>(stateIni.GetLongValue(
				animalSectionName,
				std::to_string(animalHash).c_str(),
				static_cast<long>(QualityFilter::PERFECT)
			))
		};
		long filterValue = qualityFilter.GetBitMask();
		animalInfos.insert({ 
			animalHash, 
			AnimalInfo{
				animalHash, 
				value, 
				fish.count(animalHash) > 0, 
				std::move(qualityFilter)
			} 
		});
	}
	StoreAnimalMap(animalInfos);
	return animalInfos;
}

void IniOptions::StoreAnimalMap(std::unordered_map<Hash, AnimalInfo> const& animalMap) const{
	CSimpleIniA stateIni;
	for (auto const& entry : animalMap) {
		static_cast<void>(stateIni.SetLongValue(
			animalSectionName,
			std::to_string(entry.first).c_str(),
			static_cast<long>(entry.second.GetQualityBitmask()),
			entry.second.GetName().c_str(),
			true
		));
		// TODO log this somewhere (logfile?)
	}
	stateIni.SaveFile(m_stateFilePath.c_str());
	// TODO log this somewhere (logfile?)
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