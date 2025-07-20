#pragma once

#include "IInfo.h"
#include "AnimalInfo.h"

#include <vector>
#include <memory>

class IContainedInfo;
class ContainedInfoAccess;

class IContainingInfo : virtual public IInfo {
public:
	virtual ~IContainingInfo() = default;

	virtual void AddContainedItem(std::unique_ptr<IContainedInfo> child) = 0;
	virtual void UnsetQuality(std::vector<const IInfo*> & affectedInfos, ContainedInfoAccess const&) = 0;
	virtual std::vector<AnimalInfo*> GetAllAnimalInfos() = 0;
};

class ContainingInfoAccess {
	friend class IContainingInfo;
private:
	ContainingInfoAccess() = default;
};