#pragma once
#ifndef INI_OPTIONS_H
#define INI_OPTIONS_H
#include "..\iniReader\SimpleIni.h"
#include "..\inc\types.h";

class IniOptions
{
protected:
	CSimpleIniA generalIni{};
	bool showBirds;
	bool showExcellentQuality;
	bool showMediumQuality;
	bool showPoorQuality;
	bool getBoolIniValue(const char* value);
	IniOptions& loadOptions();
public:
	IniOptions();
	std::map<Hash, const char*> getAnimalsNames();
	bool getShowBirds();
	bool getShowExcellentQuality();
	bool getShowMediumQuality();
	bool getShowPoorQuality();
};
#endif