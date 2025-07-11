#pragma once

#include "..\inc\types.h"
#include "..\iniReader\SimpleIni.h"

#include <experimental/filesystem>
#include <map>
#include <string>

class IniOptions
{
public:
	IniOptions( std::experimental::filesystem::path const& generalInifile);

	std::map<Hash, std::string> getAnimalsNames() const;
	bool getShowBirds() const;
	bool getShowExcellentQuality() const;
	bool getShowMediumQuality() const;
	bool getShowPoorQuality() const;

private:
	static bool getBoolIniValue(const char* value);

	static char const* const const sectionName;

	CSimpleIniA generalIni{};
	bool showBirds{ false };
	bool showExcellentQuality{ false };
	bool showMediumQuality{ false };
	bool showPoorQuality{ false };
};