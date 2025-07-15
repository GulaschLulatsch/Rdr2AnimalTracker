#include "AnimalInfo.h"

#include "QualityFilter.h"

#include <RDR2ScriptHook/types.h>
#include <RDR2ScriptHook/enums.h>

#include <string>
#include <type_traits>

AnimalInfo::AnimalInfo(Hash hash, std::string const& name, bool isFish, QualityFilter filter) :
	m_hash{ hash },
	m_name{ name },
	m_isFish{ isFish },
	m_filter{ std::move(filter) }
{}

Hash AnimalInfo::GetHash() const
{
	return m_hash;
}

const std::string& AnimalInfo::GetName() const
{
	return m_name;
}

bool AnimalInfo::IsFish() const
{
	return m_isFish;
}

bool AnimalInfo::QualityMatches(ePedQuality quality) const
{
	return m_filter.Matches(quality);
}

void AnimalInfo::RotateQuality()
{
	m_filter.Rotate();
}
