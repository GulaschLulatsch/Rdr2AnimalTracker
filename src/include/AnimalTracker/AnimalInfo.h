#pragma once

#include "Categories.h"
#include "IContainedInfo.h"
#include "IContainingInfo.h"
#include "IInfo.h"
#include "IInfoPersister.h"
#include "QualityFilter.h"

#include <RDR2ScriptHook/enums.h>

#include <string>
#include <vector>

class CategoryInfo;

class AnimalInfo : public IContainedInfo {
public:
	AnimalInfo(
		AnimalType hash,
		std::string const& name, 
		bool isFish,
		QualityFilter filter,
		IInfoPersister const& saveFile
	);

	InfoClass GetClass() const override;
	const std::string& GetKey() const override;
	const std::string& GetName() const override;
	int GetQualityBitmask() const override;
	std::vector<AnimalInfo*> GetAllAnimalInfos() override;

	Hash GetHash() const;
	bool IsFish() const;

	bool IsQualitySet() const override;
	bool QualityMatches(ePedQuality quality) const override;
	void SetContainingItem(IContainingInfo& parent, ContainingInfoAccess const&) override;
	void SetQuality(const QualityFilter& quality, std::vector<const IInfo*>& affectedInfos, ContainingInfoAccess const&) override;

	void RotateQuality() override;

	std::unique_ptr<IMenuItem> CreateMenuItem() override;

private:
	IContainingInfo* m_parentItem{ nullptr };

	const AnimalType m_hash;
	const std::string m_key;
	const std::string m_name;
	const bool m_isFish;

	QualityFilter m_filter;
	IInfoPersister const& m_saveFile;

	static const ContainedInfoAccess ACCESS;
};