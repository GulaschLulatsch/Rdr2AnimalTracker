#pragma once

#include "IContainingInfo.h"
#include "IInfo.h"
#include "IInfoPersister.h"

#include "SimpleIni\SimpleIni.h"

#include <memory>
#include <string>
#include <vector>

class IniOptions : public IInfoPersister {
public:
	IniOptions( std::string const& generalInifile);

	std::vector<std::unique_ptr<IInfo>> LoadInfo() const;
	void StoreInfos(std::vector<const IInfo*> infos) const override;

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