#include "AnimalInfo.h"

#include "Categories.h"
#include "IContainedInfo.h"
#include "IContainingInfo.h"
#include "IInfo.h"
#include "IInfoPersister.h"
#include "IMenuItem.h"
#include "MenuItemAnimal.h"
#include "QualityFilter.h"

#include <ScriptHookRDR2/enums.h>
#include <ScriptHookRDR2/types.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

AnimalInfo::AnimalInfo(AnimalType hash, std::string const& name, bool isFish, QualityFilter filter, IInfoPersister const& saveFile) :
	m_hash{ hash },
	m_key{ std::to_string(hash._value) },
	m_name{ name },
	m_isFish{ isFish },
	m_filter{ std::move(filter) },
	m_saveFile{ saveFile }
{}

Hash AnimalInfo::GetHash() const
{
	return m_hash._value;
}

const std::string& AnimalInfo::GetName() const
{
	return m_name;
}

int AnimalInfo::GetQualityBitmask() const
{
	return m_filter.GetBitMask();
}

std::vector<AnimalInfo*> AnimalInfo::GetAllAnimalInfos()
{
	return { this };
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
		m_parentItem->UnsetQuality(affectedInfos, PARENT_ACCESS);
	}
	m_saveFile.StoreInfos(std::move(affectedInfos));
}

std::unique_ptr<IMenuItem> AnimalInfo::CreateMenuItem()
{
	return std::make_unique<MenuItemAnimal>(*this);
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
