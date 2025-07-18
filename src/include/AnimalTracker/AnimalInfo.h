#pragma once

#include "IInfo.h"
#include "QualityFilter.h"

#include <RDR2ScriptHook/enums.h>
#include <RDR2ScriptHook/types.h>

#include <string>

class CategoryInfo;

class AnimalInfo : public IInfo {
public:
	AnimalInfo(
		Hash hash, 
		std::string const& name, 
		bool isFish,
		QualityFilter filter
	);

	Hash GetHash() const;
	const std::string& GetName() const override;
	int GetQualityBitmask() const override;

	bool IsFish() const;

	bool IsQualitySet() const override;
	bool QualityMatches(ePedQuality quality) const override;
	void RotateQuality() override;
	void SetContainingCategory(CategoryInfo* category);
	void SetQuality(const QualityFilter& quality);

private:
	const Hash m_hash;
	const std::string m_name;
	const bool m_isFish;
	CategoryInfo* m_parentCategory{ nullptr };
	QualityFilter m_filter;

};