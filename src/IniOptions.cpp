#include "IniOptions.h"

#include "AnimalInfo.h"
#include "NamesInfo.h"
#include "QualityFilter.h"

#include "RDR2ScriptHook/types.h"

#include "SimpleIni/SimpleIni.h"

#include <stdexcept>
#include <string>
#include <unordered_map>

char const * const IniOptions::sectionName{ "GENERAL" };

IniOptions::IniOptions(std::string const & generalInifile)
{
	if (generalIni.LoadFile(generalInifile.c_str()) < 0) {
		throw std::runtime_error{ "Error! Failed to load the ini file: " + generalInifile};
	}
	showBirds = getBoolIniValue(generalIni.GetValue(sectionName, "showBirds", nullptr));
	showExcellentQuality = getBoolIniValue(generalIni.GetValue(sectionName, "showExcellentQuality", nullptr));
	showMediumQuality = getBoolIniValue(generalIni.GetValue(sectionName, "showMediumQuality", nullptr));
	showPoorQuality = getBoolIniValue(generalIni.GetValue(sectionName, "showPoorQuality", nullptr));
}

bool IniOptions::getBoolIniValue(const char* value) {
	return (value != nullptr) && (std::string(value) == "true");
}

std::unordered_map<Hash, AnimalInfo> IniOptions::getAnimalNames() const
{
	std::string langFilePath{ std::string{ "AnimalsFinder/" } + generalIni.GetValue(sectionName, "langFilePath", "eng.ini") };

	CSimpleIniA LangIni;
	if(LangIni.LoadFile(langFilePath.c_str()) < 0) {
		throw std::runtime_error{ std::string{ "Error! Failed to load the language file: " } + langFilePath};
	};

	// Get all keys in a section
	CSimpleIniA::TNamesDepend keys;
	LangIni.GetAllKeys("LANG", keys);

	std::unordered_map<Hash, AnimalInfo> animalInfos;

	// Iterate through keys and get their values
	for (const auto& key : keys) {
		const char* value = LangIni.GetValue("LANG", key.pItem, "undefinded");

		Hash animalHash = typetoHash.at(key.pItem);
		animalInfos.insert({ 
			animalHash, 
			AnimalInfo{
				animalHash, 
				value, 
				fish.count(animalHash) > 0, 
				QualityFilter{ /*TODO read saved quality*/
					showPoorQuality,
					showMediumQuality,
					showExcellentQuality
				}
			} 
		});
	}

	return animalInfos;
}


bool IniOptions::getShowBirds() const
{
	return showBirds;
}

bool IniOptions::getShowExcellentQuality() const
{
	return showExcellentQuality;
}

bool IniOptions::getShowMediumQuality() const
{
	return showMediumQuality;
}

bool IniOptions::getShowPoorQuality() const
{
	return showPoorQuality;
}

