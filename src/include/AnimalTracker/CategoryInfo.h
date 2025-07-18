#pragma once

#include "IInfo.h"
#include "QualityFilter.h"

#include <RDR2ScriptHook/enums.h>

#include <string>
#include <vector>

class AnimalInfo;

class CategoryInfo : public IInfo{
public:
	CategoryInfo(
		std::string const& name,
		QualityFilter filter 
	);

	void AddAnimal(AnimalInfo* animal);

	const std::string& GetName() const override;
	int GetQualityBitmask() const override;
	bool IsQualitySet() const override;
	bool QualityMatches(ePedQuality quality) const override;

	void RotateQuality() override;
	void UnsetQuality();

private:
	const std::string m_name;
	QualityFilter m_filter;
	std::vector<AnimalInfo*> m_animals;
};