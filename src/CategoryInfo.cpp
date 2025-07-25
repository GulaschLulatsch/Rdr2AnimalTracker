#include "CategoryInfo.h"

#include "AnimalInfo.h"
#include "CategoryMenu.h"
#include "IContainedInfo.h"
#include "IContainingInfo.h"
#include "IInfo.h"
#include "IInfoPersister.h"
#include "IMenuItem.h"
#include "MenuItemCategory.h"
#include "QualityFilter.h"

#include <RDR2ScriptHook/enums.h>

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

const ContainedInfoAccess CategoryInfo::PARENT_ACCESS{};
const ContainingInfoAccess CategoryInfo::CHILD_ACCESS{};

CategoryInfo::CategoryInfo(
	std::string const& key,
	std::string const& name,
	QualityFilter filter,
	IInfoPersister const& saveFile
) :
	m_key{ key },
	m_name{ name },
	m_filter{ std::move(filter) },
	m_saveFile{ saveFile }
{}

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
	std::vector<const IInfo*> affectedItems{ this };
	for (auto const& child : m_children) {
		child->SetQuality(m_filter, affectedItems, CHILD_ACCESS);
	}
	if (m_parentItem) {
		m_parentItem->UnsetQuality(affectedItems, PARENT_ACCESS);
	}
	m_saveFile.StoreInfos(std::move(affectedItems));
}

std::unique_ptr<IMenuItem> CategoryInfo::CreateMenuItem()
{
	std::vector<std::unique_ptr<IMenuItem>> items{};
	for (const auto& info : m_children) {
		items.emplace_back(info->CreateMenuItem());
	}

	std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return a->GetCaption() < b->GetCaption(); });

	auto menu{ std::make_unique<CategoryMenu>(nullptr) };
	for (auto& item : items) {
		menu->AddItem(std::move(item));
	}

	return std::make_unique<MenuItemCategory>(*this, std::move(menu));
}

InfoClass CategoryInfo::GetClass() const
{
	return InfoClass::CategoryInfo;
}

const std::string& CategoryInfo::GetKey() const
{
	return m_key;
}

void CategoryInfo::SetContainingItem(IContainingInfo& parent, ContainingInfoAccess const&)
{
	m_parentItem = &parent;
}

std::vector<AnimalInfo*> CategoryInfo::GetAllAnimalInfos()
{
	std::vector<AnimalInfo*> returnVector{};
	for (auto& entry : m_children) {
		auto vec{ entry->GetAllAnimalInfos() };
		returnVector.insert(returnVector.end(), vec.begin(), vec.end());
	}
	return returnVector;
}

void CategoryInfo::SetQuality(const QualityFilter& quality, std::vector<const IInfo*>& affectedInfos, ContainingInfoAccess const&)
{
	m_filter = quality;
	affectedInfos.push_back(this);
}

void CategoryInfo::AddContainedItem(std::unique_ptr<IContainedInfo> child)
{
	child->SetContainingItem(*this, CHILD_ACCESS);
	m_children.push_back(std::move(child));
}

void CategoryInfo::UnsetQuality(std::vector<const IInfo*>& affectedInfos, ContainedInfoAccess const&)
{
	m_filter.Unset();
	affectedInfos.push_back(this);
}
