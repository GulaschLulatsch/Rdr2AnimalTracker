#pragma once

#include "ButtonMapping.h"
#include "IInfo.h"
#include "IInfoPersister.h"

#include <SimpleIni\SimpleIni.h>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

class IniOptions : public IInfoPersister {
public:
	IniOptions(std::filesystem::path const& generalInifile);

	std::vector<std::unique_ptr<IInfo>> LoadInfo() const;
	std::vector<ButtonMapping> LoadButtonMappings() const;
	void StoreInfos(std::vector<const IInfo*> infos) const override;
	const char* GetLocale() const;

private:
	static char const* const generalSectionName;
	static char const* const languageSectionName;
	static char const* const animalSectionName;
	static char const* const categorySectionName;
	static char const* const buttonMappingsSectionName;
	static char const* const withControlPrefix;
	static char const* const withAltPrefix;
	static char const* const withShiftPrefix;

	static std::string ExpandEnvironmentVariables(const std::string& input); 

	CSimpleIniA m_generalIni;
	std::filesystem::path m_langFilePath{};
	std::filesystem::path m_stateFilePath{};
	std::string m_locale;
};