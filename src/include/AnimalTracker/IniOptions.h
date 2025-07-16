#pragma once

#include "RDR2ScriptHook\types.h"

#include "AnimalInfo.h"

#include "SimpleIni\SimpleIni.h"

#include <unordered_map>
#include <string>

class IniOptions
{
public:
	IniOptions( std::string const& generalInifile);

	std::unordered_map<Hash, AnimalInfo> GetAnimalMap() const;
	void StoreAnimalMap(std::unordered_map<Hash, AnimalInfo> const& animalMap) const;

private:
	static char const* const generalSectionName;
	static char const* const languageSectionName;
	static char const* const animalSectionName;
	static char const* const categorySectionName;

	static std::string ExpandEnvironmentVariables(const std::string& input); 

	CSimpleIniA m_generalIni{};
	std::string m_langFilePath{};
	std::string m_stateFilePath{};
};