#pragma once

#include "RDR2ScriptHook\types.h"

#include "SimpleIni\SimpleIni.h"

#include <map>
#include <string>

class IniOptions
{
public:
	IniOptions( std::string const& generalInifile);

	std::map<Hash, std::string> getAnimalNames() const;
	bool getShowBirds() const;
	bool getShowExcellentQuality() const;
	bool getShowMediumQuality() const;
	bool getShowPoorQuality() const;

private:
	static bool getBoolIniValue(const char* value);

	static char const* const sectionName;

	CSimpleIniA generalIni{};
	bool showBirds{ false };
	bool showExcellentQuality{ false };
	bool showMediumQuality{ false };
	bool showPoorQuality{ false };
};