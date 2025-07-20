#include "AnimalInfo.h"

#include "IContainedInfo.h"
#include "IContainingInfo.h"
#include "IInfo.h"
#include "IInfoPersister.h"
#include "QualityFilter.h"

#include <RDR2ScriptHook/enums.h>
#include <RDR2ScriptHook/types.h>

#include <string>
#include <utility>
#include <vector>

const ContainedInfoAccess AnimalInfo::ACCESS{};

AnimalInfo::AnimalInfo(AnimalType hash, std::string const& name, bool isFish, QualityFilter filter, IInfoPersister const& saveFile) :
	m_hash{ hash },
	m_key{ std::to_string(hash._value) },
	m_name{ name },
	m_isFish{ isFish },
	m_filter{ std::move(filter) },
	m_saveFile{ saveFile }
{}

AnimalType AnimalInfo::GetType() const
{
	return m_hash;
}

const std::string& AnimalInfo::GetName() const
{
	return m_name;
}

int AnimalInfo::GetQualityBitmask() const
{
	return m_filter.GetBitMask();
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
	std::vector<const IInfo*> affectedInfos{ this };

	if (m_parentItem) {
		m_parentItem->UnsetQuality(affectedInfos, ACCESS);
	}
	m_saveFile.StoreInfos(std::move(affectedInfos));
}

void AnimalInfo::SetContainingItem(IContainingInfo& parent, ContainingInfoAccess const&)
{
	m_parentItem = &parent;
}

void AnimalInfo::SetQuality(const QualityFilter& quality, std::vector<const IInfo*>& affectedInfos, ContainingInfoAccess const&)
{
	m_filter = quality;
	affectedInfos.push_back(this);
}

InfoClass AnimalInfo::GetClass() const
{
	return InfoClass::AnimalInfo;
}

const std::string& AnimalInfo::GetKey() const
{
	return m_key;
}

bool AnimalInfo::IsQualitySet() const
{
	return m_filter.IsSet();
}
