#pragma once

#include "IContainedInfo.h"
#include "IContainingInfo.h"
#include "IInfo.h"
#include "IInfoPersister.h"
#include "IMenuItem.h"
#include "ISandwichedInfo.h"
#include "QualityFilter.h"

#include <ScriptHookRDR2/enums.h>

#include <memory>
#include <string>
#include <vector>

class AnimalInfo;

class CategoryInfo : public ISandwichedInfo {
public:
	CategoryInfo(
		std::string const& key,
		std::string const& name,
		QualityFilter filter,
		IInfoPersister const& saveFile
	);

	InfoClass GetClass() const override;
	const std::string& GetKey() const override;
	const std::string& GetName() const override;
	int GetQualityBitmask() const override;

	bool IsQualitySet() const override;
	bool QualityMatches(ePedQuality quality) const override;

	void AddContainedItem(std::unique_ptr<IContainedInfo> child) override;
	void SetContainingItem(IContainingInfo& parent, ContainingInfoAccess const&) override;
	std::vector<AnimalInfo*> GetAllAnimalInfos() override;

	void SetQuality(const QualityFilter& quality, std::vector<const IInfo*>& affectedInfos, ContainingInfoAccess const&) override;
	void UnsetQuality(std::vector<const IInfo*>& affectedInfos, ContainedInfoAccess const&) override;

	void RotateQuality() override;

	std::unique_ptr<IMenuItem> CreateMenuItem() override;

private:
	IContainingInfo* m_parentItem{ nullptr };

	const std::string m_key;
	const std::string m_name;

	QualityFilter m_filter;
	IInfoPersister const& m_saveFile;

	std::vector<std::unique_ptr<IContainedInfo>> m_children;
};