#include "CategoryInfo.h"

#include "AnimalInfo.h"
#include "QualityFilter.h"

#include <RDR2ScriptHook/enums.h>

#include <string>
#include <utility>

CategoryInfo::CategoryInfo(std::string const& name, QualityFilter filter) :
	m_name{ name },
	m_filter{ std::move(filter) }
{}

void CategoryInfo::AddAnimal(AnimalInfo * animal)
{
	animal->SetContainingCategory(this);
	m_animals.push_back(animal);
}

const std::string& CategoryInfo::GetName() const {
	return m_name;
}

int CategoryInfo::GetQualityBitmask() const {
	return m_filter.GetBitMask();
}

bool CategoryInfo::IsQualitySet() const{
	return m_filter.IsSet();
}

bool CategoryInfo::QualityMatches(ePedQuality quality) const
{
	return m_filter.Matches(quality);
}

void CategoryInfo::RotateQuality() {
	m_filter.Rotate();
	for (auto const& animal : m_animals) {
		animal->SetQuality(m_filter);
	}
}

void CategoryInfo::UnsetQuality() {
	m_filter.Unset();
}